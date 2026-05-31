//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_class_member_instantiation_693
 */ 
class MutatorFrontendAction_693 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(693)

private:
    class MutatorASTConsumer_693 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_693(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

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

        const clang::CXXRecordDecl *RD = TSD->getTemplateName().getAsTemplateDecl()->getTemplatedDecl();
        if (!RD || !RD->isCompleteDefinition())
            return;

        for (const auto *D : RD->decls()) {
            if (const auto *VD = llvm::dyn_cast<clang::VarDecl>(D)) {
                if (VD->getType().isConstexpr() && VD->getType()->isSameType(TSD->getArg(0).getAsType())) {
                    std::string newMemberName = VD->getNameAsString();
                    std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
                    std::string newText = originalText;
                    size_t pos = newText.find(TSD->getArg(0).getAsExpr()->getNameAsString());
                    if (pos != std::string::npos) {
                        newText.replace(pos, TSD->getArg(0).getAsExpr()->getNameAsString().length(), newMemberName);
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