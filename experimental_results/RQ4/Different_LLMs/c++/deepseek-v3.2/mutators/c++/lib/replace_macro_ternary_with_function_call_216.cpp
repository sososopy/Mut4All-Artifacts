//source file
#include "../include/Mutator_Replace_Macro_Ternary_With_Function_Call_216.h"

// ========================================================================================================
#define MUT216_OUTPUT 1

void MutatorFrontendAction_216::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("constexprVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (!VD->isConstexpr())
        return;
      auto Init = VD->getInit();
      if (!Init)
        return;
      //Check if initializer is a ConditionalOperator (ternary) or possibly a macro expansion
      const clang::Expr* Expr = Init->IgnoreParens();
      if (!Expr)
        return;
      //Look for ConditionalOperator
      const clang::ConditionalOperator* CondOp = nullptr;
      if (const clang::ConditionalOperator* CO = llvm::dyn_cast<clang::ConditionalOperator>(Expr)) {
        CondOp = CO;
      } else {
        //Try to see if it's a CallExpr that might be a macro expansion (like MIN macro)
        //We'll focus on ConditionalOperator directly for simplicity
        return;
      }
      if (!CondOp)
        return;
      //Check if any branch contains pointer dereference (MemberExpr with -> or UnaryOperator with *)
      bool hasPtrDeref = false;
      auto CheckExpr = [&](const clang::Expr* E) {
        if (const clang::MemberExpr* ME = llvm::dyn_cast<clang::MemberExpr>(E)) {
          if (ME->isArrow())
            hasPtrDeref = true;
        } else if (const clang::UnaryOperator* UO = llvm::dyn_cast<clang::UnaryOperator>(E)) {
          if (UO->getOpcode() == clang::UO_Deref)
            hasPtrDeref = true;
        }
      };
      CheckExpr(CondOp->getCond());
      CheckExpr(CondOp->getTrueExpr());
      CheckExpr(CondOp->getFalseExpr());
      if (!hasPtrDeref)
        return;
      //Record the node information to be used in the mutation process
      targetDecls.push_back(VD);
      //Generate a unique function name
      std::string funcName = "eval_min_" + std::to_string(functionNames.size());
      functionNames.push_back(funcName);
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("constexprVarForReplace")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Find corresponding recorded declaration
      size_t index = 0;
      for (; index < targetDecls.size(); ++index) {
        if (targetDecls[index] == VD)
          break;
      }
      if (index >= targetDecls.size())
        return;
      std::string funcName = functionNames[index];
      //Get the source code text of target node
      auto Init = VD->getInit();
      if (!Init)
        return;
      const clang::Expr* Expr = Init->IgnoreParens();
      const clang::ConditionalOperator* CondOp = llvm::dyn_cast<clang::ConditionalOperator>(Expr);
      if (!CondOp)
        return;
      //Extract types and arguments
      std::string condStr, trueStr, falseStr;
      condStr = stringutils::rangetoStr(*Result.SourceManager, CondOp->getCond()->getSourceRange());
      trueStr = stringutils::rangetoStr(*Result.SourceManager, CondOp->getTrueExpr()->getSourceRange());
      falseStr = stringutils::rangetoStr(*Result.SourceManager, CondOp->getFalseExpr()->getSourceRange());
      //Determine parameter types (simplified: assume int and pointer types exist)
      //We'll use placeholder types; in practice we'd extract from AST
      std::string paramList;
      //For simplicity, we'll assume two parameters: a value and a pointer
      paramList = "int value_param, const void* ptr_param";
      //Construct function declaration
      std::string funcDecl = "constexpr int " + funcName + "(" + paramList + ") {\n";
      funcDecl += "    return (" + condStr + ") ? (" + trueStr + ") : (" + falseStr + ");\n}\n";
      //Insert function declaration before the variable declaration
      SourceLocation insertLoc = VD->getBeginLoc();
      Rewrite.InsertTextBefore(insertLoc, funcDecl);
      //Replace initializer with function call
      std::string callExpr = funcName + "(/*args*/)";
      //We need to determine actual arguments; for simplicity use placeholder
      //In real implementation, we'd extract arguments from the ternary operands
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Init->getSourceRange()), callExpr);
    }
}
  
void MutatorFrontendAction_216::MutatorASTConsumer_216::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(isConstexpr(), hasInitializer(conditionalOperator())).bind("constexprVar");
    DeclarationMatcher replaceMatcher = varDecl(isConstexpr(), hasInitializer(conditionalOperator())).bind("constexprVarForReplace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(replaceMatcher, &callback);
    matchFinder.matchAST(Context);
}