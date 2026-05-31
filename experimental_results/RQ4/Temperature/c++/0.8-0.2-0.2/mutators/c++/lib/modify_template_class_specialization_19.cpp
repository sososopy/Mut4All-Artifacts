//source file
#include "../include/modify_template_class_specialization_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (const auto *Spec = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("PartialSpec")) {
      //Filter nodes in header files
      if (!Result.Context->getSourceManager().isWrittenInMainFile(Spec->getLocation()))
        return;
      
      //Get the source code text of target node
      auto specSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                Spec->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string modifiedSpec = specSource;
      size_t pos = modifiedSpec.find("typename");
      if (pos != std::string::npos) {
          modifiedSpec.replace(pos, 8, "typename T");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Spec->getSourceRange()), modifiedSpec);
    }
}
  
void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("PartialSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}