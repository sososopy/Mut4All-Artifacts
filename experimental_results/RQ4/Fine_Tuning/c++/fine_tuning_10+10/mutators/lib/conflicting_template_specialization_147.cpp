//source file
#include "../include/conflicting_template_specialization_14747.h"

// ========================================================================================================
#define MUT147_OUTPUT 1

void MutatorFrontendAction_14747::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;

        std::string templateName = CT->getNameAsString();
        if (specializedTemplates.find(templateName) != specializedTemplates.end())
            return;

        specializedTemplates.insert(templateName);
        std::string specializationCode = "template<typename T> class " + templateName + "<T*> {};";

        SourceLocation insertLoc = CT->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, "\n/*mut147*/" + specializationCode);
    }
}

void MutatorFrontendAction_14747::MutatorASTConsumer_14747::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}