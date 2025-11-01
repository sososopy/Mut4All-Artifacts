//source file
#include "../include/Template_Specialization_Conflict_54.h"

// ========================================================================================================
#define MUT54_OUTPUT 1

void MutatorFrontendAction_54::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpec")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (visitedTemplates.find(TD->getSpecializedTemplate()) != visitedTemplates.end())
            return;

        visitedTemplates.insert(TD->getSpecializedTemplate());

        std::string templateName = TD->getSpecializedTemplate()->getNameAsString();
        std::string newSpec = "template<> class " + templateName + "<int, double> { public: void doSomething(); };";
        SourceLocation insertLoc = TD->getEndLoc();
        Rewrite.InsertTextAfterToken(insertLoc, "\n/*mut54*/" + newSpec);
    }
}
  
void MutatorFrontendAction_54::MutatorASTConsumer_54::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}