//header file
#pragma once
#include "Mutator_base.h"

/**
 * scoped_static_variable_in_template_argument_633
 */ 
class MutatorFrontendAction_633 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(633)

private:
    class MutatorASTConsumer_633 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_633(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/scoped_static_variable_in_template_argument_633.h"

// ========================================================================================================
#define MUT633_OUTPUT 1

void MutatorFrontendAction_633::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithStatic")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto body = FD->getBody();
            for (auto &stmt : body->children()) {
                if (auto *DS = llvm::dyn_cast<clang::DeclStmt>(stmt)) {
                    for (auto *decl : DS->decls()) {
                        if (auto *VD = llvm::dyn_cast<clang::VarDecl>(decl)) {
                            if (VD->isStaticLocal()) {
                                std::string staticVarName = VD->getNameAsString();
                                std::string templateVarDecl = "template <const int& Reference>\nstatic auto templateVar = 0;";
                                std::string useTemplateVar = "auto useTemplate = templateVar<" + staticVarName + ">;";
                                std::string modifyStaticVar = staticVarName + " = 20;";

                                SourceLocation insertLoc = FD->getBodyRBrace();
                                Rewrite.InsertTextBefore(insertLoc, "\n" + templateVarDecl + "\n" + useTemplateVar + "\n" + modifyStaticVar + "\n");
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_633::MutatorASTConsumer_633::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasDescendant(varDecl(isStaticLocal()).bind("StaticVar"))).bind("FunctionWithStatic");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}