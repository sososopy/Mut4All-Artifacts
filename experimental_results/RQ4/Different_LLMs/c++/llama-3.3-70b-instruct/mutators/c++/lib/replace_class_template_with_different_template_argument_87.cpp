//source file
#include "../include/Replace_Class_Template_With_Different_Template_Argument_87.h"

// ========================================================================================================
#define MUT87_OUTPUT 1

void MutatorFrontendAction_87::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("ClassTemplateSpecialization")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace the template argument with a different one
      std::string templateArgument = MT->getTemplateArgs().get(0).getAsType().getAsString();
      std::string newTemplateArgument = "int"; // Replace with a different template argument
      declaration.replace(declaration.find(templateArgument), templateArgument.length(), newTemplateArgument);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_87::MutatorASTConsumer_87::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("ClassTemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}