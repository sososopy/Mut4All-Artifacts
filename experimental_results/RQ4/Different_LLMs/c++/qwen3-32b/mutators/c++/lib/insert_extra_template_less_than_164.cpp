//source file
#include "../include/Insert_Extra_Template_Less_Than_164.h"

// ========================================================================================================
#define MUT164_OUTPUT 1

void MutatorFrontendAction_164::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("templateVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        auto *TSI = VD->getTypeSourceInfo();
        if (!TSI)
            return;
        auto TL = TSI->getTypeLoc();
        if (auto *TST = TL.getType()->getAs<clang::TemplateSpecializationType>()) {
            SourceRange typeRange = TL.getSourceRange();
            std::string originalText = stringutils::rangetoStr(*Result.SourceManager, typeRange);
            size_t lessThanPos = originalText.find('<');
            if (lessThanPos != std::string::npos) {
                originalText.insert(lessThanPos + 1, "<");
                Rewrite.ReplaceText(typeRange, originalText);
            }
        }
    }
}
  
void MutatorFrontendAction_164::MutatorASTConsumer_164::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(templateSpecializationType())).bind("templateVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}