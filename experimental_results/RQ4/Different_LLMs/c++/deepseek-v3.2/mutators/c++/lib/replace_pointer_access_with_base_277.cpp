//source file
#include "../include/Mutator_Replace_Pointer_Access_With_Base_277.h"

// ========================================================================================================
#define MUT277_OUTPUT 1

void MutatorFrontendAction_277::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MA = Result.Nodes.getNodeAs<clang::MemberExpr>("MemberExpr")) {
      //Filter nodes in header files
      if (!MA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MA->getBeginLoc()))
        return;
      //Check if it's a pointer access
      if (!MA->isArrow())
        return;
      
      //Get the base expression type
      clang::Expr* baseExpr = MA->getBase();
      if (!baseExpr)
        return;
      
      clang::QualType baseType = baseExpr->getType();
      if (!baseType->isPointerType())
        return;
      
      clang::QualType pointeeType = baseType->getPointeeType();
      if (pointeeType->isVoidType())
        return;
      
      //Find suitable non-pointer base
      clang::VarDecl* suitableBase = nullptr;
      for (const clang::VarDecl* var : nonPointerVariables) {
        if (!var->getType()->isPointerType() && var->getType().getCanonicalType() == pointeeType.getCanonicalType()) {
          suitableBase = const_cast<clang::VarDecl*>(var);
          break;
        }
      }
      
      //If no suitable base exists, create a local variable declaration
      std::string baseName;
      if (!suitableBase) {
        //Generate a unique name
        baseName = "mut277_base_" + std::to_string(getrandom::getRandomIndex(1000));
        //Insert declaration before the member expression
        std::string declText;
        if (pointeeType->isRecordType()) {
          declText = pointeeType.getAsString() + " " + baseName + "{};\n";
        } else {
          declText = pointeeType.getAsString() + " " + baseName + " = {};\n";
        }
        Rewrite.InsertTextBefore(MA->getBeginLoc(), "/*mut277*/" + declText);
        //Record the new variable
        suitableBase = nullptr; // We'll use the name directly
      } else {
        baseName = suitableBase->getNameAsString();
      }
      
      //Get the source code text of target node
      std::string memberName = MA->getMemberDecl()->getNameAsString();
      std::string newExpr = baseName + "." + memberName;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MA->getSourceRange()), newExpr);
    }
    //Also collect non-pointer variables for later use
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (!VD->getType()->isPointerType()) {
        nonPointerVariables.push_back(VD);
      }
    }
}
  
void MutatorFrontendAction_277::MutatorASTConsumer_277::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto memberMatcher = memberExpr(isArrow()).bind("MemberExpr");
    auto varMatcher = varDecl().bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(memberMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.matchAST(Context);
}