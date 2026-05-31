//source file
#include "../include/incomplete_class_definition_with_template_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        
        if (TD->getTemplatedDecl()->isCompleteDefinition())
            return;

        incompleteTemplates.push_back(TD);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        for (const auto *TD : incompleteTemplates) {
            std::string templateName = TD->getNameAsString();
            std::string mutation = "/*mut33*/ " + templateName + "<int> obj; obj.someMemberFunction();\n";
            Rewrite.InsertTextBefore(FD->getBody()->getBeginLoc(), mutation);
        }
    }
}
  
void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("TemplateDecl");
    DeclarationMatcher functionMatcher = functionDecl(hasBody(compoundStmt())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}