//source file
#include "../include/Mutator_Modify_Function_Template_Specialization_438.h"

// ========================================================================================================
#define MUT438_OUTPUT 1

void MutatorFrontendAction_438::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getTemplatedDecl()->hasBody()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        
        std::string nestedTemplate = "template <template <typename> class NestedTemplate, typename T>\n";
        std::string requiresClause = "requires requires { typename NestedTemplate<T>::type; }\n";
        
        declaration.insert(declaration.find("template <typename T>"), nestedTemplate);
        declaration.insert(declaration.find("{"), requiresClause);
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_438::MutatorASTConsumer_438::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}