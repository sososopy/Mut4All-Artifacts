//source file
#include "../include/Modify_Template_Specialization_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTSD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpec")) {
        if (!CTSD || !Result.Context->getSourceManager().isWrittenInMainFile(CTSD->getLocation()))
            return;
        
        const ClassTemplateDecl *CTD = CTSD->getSpecializedTemplate();
        if (!CTD)
            return;

        std::string templateName = CTD->getNameAsString();
        std::string newSpecialization = "template<typename T1, typename T2>\nclass " + templateName + "<T1, T2> {};\n";
        SourceLocation insertLoc = CTD->getEndLoc().getLocWithOffset(1);

        Rewrite.InsertText(insertLoc, "/*mut19*/\n" + newSpecialization);
    }
}

void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}