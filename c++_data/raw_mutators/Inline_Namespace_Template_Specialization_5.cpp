//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inline_Namespace_Template_Specialization_5
 */ 
class MutatorFrontendAction_5 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(5)

private:
    class MutatorASTConsumer_5 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_5(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/inline_namespace_template_specialization_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("inlineNamespace")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;

        for (auto *D : NS->decls()) {
            if (auto *TD = llvm::dyn_cast<clang::FunctionTemplateDecl>(D)) {
                if (TD->getTemplatedDecl()->isDefined()) {
                    auto funcName = TD->getNameAsString();
                    std::string specialization = "\n/*mut5*/template <> int " + funcName + "<int>(int);\n";
                    Rewrite.InsertTextAfterToken(NS->getEndLoc(), specialization);
                }
            }
        }
    }
}

void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(isInline()).bind("inlineNamespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}