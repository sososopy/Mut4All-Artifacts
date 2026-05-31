//source file
#include "../include/incomplete_class_template_specialization_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
            return;

        templateDecls.push_back(CTD);
        
        // Create an incomplete specialization
        std::string specialization = "template<> class " + CTD->getNameAsString() + "<int>;";
        SourceLocation insertLoc = CTD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, "\n/*mut33*/" + specialization + "\n");
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (!templateDecls.empty()) {
            const auto *CTD = templateDecls.back();
            std::string usage = CTD->getNameAsString() + "<int> " + VD->getNameAsString() + ";";
            Rewrite.ReplaceText(VD->getSourceRange(), "/*mut33*/" + usage);
        }
    }
}

void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher classTemplateMatcher = classTemplateDecl().bind("ClassTemplate");
    DeclarationMatcher varDeclMatcher = varDecl().bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classTemplateMatcher, &callback);
    matchFinder.addMatcher(varDeclMatcher, &callback);
    matchFinder.matchAST(Context);
}