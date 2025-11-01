//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alias_Template_Instantiation_In_Class_Templates_311
 */ 
class MutatorFrontendAction_311 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(311)

private:
    class MutatorASTConsumer_311 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_311(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Alias_Template_Instantiation_In_Class_Templates_311.h"

// ========================================================================================================
#define MUT311_OUTPUT 1

void MutatorFrontendAction_311::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                        CTD->getLocation()))
            return;

        for (auto *Spec : CTD->specializations()) {
            if (!Spec->isCompleteDefinition())
                continue;

            for (auto *D : Spec->decls()) {
                if (auto *Alias = llvm::dyn_cast<clang::TypeAliasTemplateDecl>(D)) {
                    std::string aliasName = Alias->getNameAsString();
                    std::string instantiation = "template<> " + aliasName + "<>::type;";
                    Rewrite.InsertTextAfter(Spec->getRBraceLoc(), "\n/*mut311*/" + instantiation);
                }
            }
        }
    }
}

void MutatorFrontendAction_311::MutatorASTConsumer_311::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(hasDescendant(typeAliasTemplateDecl())).bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}