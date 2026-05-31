//source file
#include "../include/modify_template_class_member_instantiation_693.h"

// ========================================================================================================
#define MUT693_OUTPUT 1

void MutatorFrontendAction_693::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("templateVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;

        const auto *TSD = VD->getType()->getAs<clang::TemplateSpecializationType>();
        if (!TSD)
            return;

        const clang::ClassTemplateSpecializationDecl *RD = llvm::dyn_cast<clang::ClassTemplateSpecializationDecl>(TSD->getTemplateName().getAsTemplateDecl());
        if (!RD || !RD->isCompleteDefinition())
            return;

        for (const auto *D : RD->decls()) {
            if (const auto *VD = llvm::dyn_cast<clang::VarDecl>(D)) {
                if (VD->isConstexpr() && VD->getType() == TSD->getArg(0).getAsType()) {
                    std::string newMemberName = VD->getNameAsString();
                    std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
                    std::string newText = originalText;
                    size_t pos = newText.find(VD->getNameAsString());
                    if (pos != std::string::npos) {
                        newText.replace(pos, VD->getNameAsString().length(), newMemberName);
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), newText);
                    }
                    break;
                }
            }
        }
    }
}
  
void MutatorFrontendAction_693::MutatorASTConsumer_693::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(templateSpecializationType())).bind("templateVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}