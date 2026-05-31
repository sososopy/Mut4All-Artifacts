//source file
#include "../include/template_instantiation_with_unused_type_parameter_456.h"

// ========================================================================================================
#define MUT456_OUTPUT 456

void MutatorFrontendAction_456::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      if (auto *RD = TD->getTemplatedDecl()) {
        for (auto *D : RD->decls()) {
          if (auto *MD = llvm::dyn_cast<clang::CXXMethodDecl>(D)) {
            if (MD->hasBody()) {
              //Get the source code text of target node
              auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                         MD->getSourceRange());
              //Perform mutation on the source code text by applying string replacement
              std::string typeParamName = TD->getTemplateParameters()->getParam(0)->getNameAsString();
              std::string dummyVar = typeParamName + " dummyVar;";
              declaration.insert(declaration.find("{") + 1, "\n/*mut456*/" + dummyVar + "\n");
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(MD->getSourceRange()), declaration);
              break;
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_456::MutatorASTConsumer_456::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}