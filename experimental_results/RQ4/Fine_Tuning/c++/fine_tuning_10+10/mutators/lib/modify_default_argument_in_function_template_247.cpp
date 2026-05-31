//source file
#include "../include/Modify_Default_Argument_In_Function_Template_247.h"

// ========================================================================================================
#define MUT247_OUTPUT 1

void MutatorFrontendAction_247::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (auto *FTD = FD->getTemplatedDecl()) {
        for (auto *param : FTD->parameters()) {
          if (auto *PD = llvm::dyn_cast<clang::ParmVarDecl>(param)) {
            if (PD->hasDefaultArg()) {
              //Get the source code text of target node
              auto defaultArg = PD->getDefaultArg()->getSourceRange();
              auto defaultArgText = stringutils::rangetoStr(*(Result.SourceManager), defaultArg);
              //Perform mutation on the source code text by applying string replacement
              std::string newDefaultArg = "recursiveFunc(" + PD->getNameAsString() + ")";
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(defaultArg, newDefaultArg);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_247::MutatorASTConsumer_247::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(parmVarDecl(hasDefaultArgument()))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}