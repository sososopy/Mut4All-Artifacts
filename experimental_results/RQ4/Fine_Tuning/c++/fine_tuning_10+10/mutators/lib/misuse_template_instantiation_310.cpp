//source file
#include "../include/misuse_template_instantiation_310.h"

// ========================================================================================================
#define MUT310_OUTPUT 1

void MutatorFrontendAction_310::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        auto typeLoc = VD->getTypeSourceInfo()->getTypeLoc();
        if (auto templateSpecTypeLoc = typeLoc.getAs<clang::TemplateSpecializationTypeLoc>()) {
            auto templateName = templateSpecTypeLoc.getTypePtr()->getTemplateName().getAsTemplateDecl()->getNameAsString();
            auto sourceRange = templateSpecTypeLoc.getSourceRange();
            std::string replacement = "std::" + templateName + " ";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), replacement);
        }
    }
}

void MutatorFrontendAction_310::MutatorASTConsumer_310::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(templateSpecializationType())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}