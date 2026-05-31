//source file
#include "../include/Complex_Number_Builtin_Operator_Mutation_477.h"

// ========================================================================================================
#define MUT477_OUTPUT 1

void MutatorFrontendAction_477::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("complexVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto type = VD->getType();
      if (type->isComplexType()) {
        //Record the node information to be used in the mutation process
        complexVars.push_back(VD);
      }
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      //Get the source code text of target node
      for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
        auto arg = CE->getArg(i);
        if (auto *UE = dyn_cast<clang::UnaryExprOrTypeTraitExpr>(arg)) {
          if (UE->getKind() != clang::UETT_SizeOf && UE->getKind() != clang::UETT_AlignOf)
            continue;
          auto subExpr = UE->getArgumentExpr();
          if (auto *DRE = dyn_cast<clang::DeclRefExpr>(subExpr)) {
            auto decl = DRE->getDecl();
            if (auto *VD = dyn_cast<clang::VarDecl>(decl)) {
              if (VD->getType()->isComplexType()) {
                //Perform mutation on the source code text by applying string replacement
                std::string op = UE->getKind() == clang::UETT_SizeOf ? "__real__" : "__imag__";
                std::string mutated = "0 < &" + op + " " + VD->getNameAsString();
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(arg->getSourceRange()), mutated);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_477::MutatorASTConsumer_477::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varMatcher = varDecl().bind("complexVar");
    StatementMatcher callMatcher = callExpr().bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}