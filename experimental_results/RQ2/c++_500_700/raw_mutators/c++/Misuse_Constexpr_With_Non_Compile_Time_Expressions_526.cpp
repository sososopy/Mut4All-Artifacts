//header file
#pragma once
#include "Mutator_base.h"

/**
 * misuse_constexpr_with_non_compile_time_expressions_526
 */ 
class MutatorFrontendAction_526 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(526)

private:
    class MutatorASTConsumer_526 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_526(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misuse_constexpr_with_non_compile_time_expressions_526.h"

// ========================================================================================================
#define MUT526_OUTPUT 1

void MutatorFrontendAction_526::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("ConstexprFunction")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                FD->getLocation()))
            return;

        if (FD->isConstexpr()) {
            for (auto *BodyStmt : FD->getBody()->children()) {
                if (auto *DS = llvm::dyn_cast<clang::DeclStmt>(BodyStmt)) {
                    for (auto *D : DS->decls()) {
                        if (auto *VD = llvm::dyn_cast<clang::VarDecl>(D)) {
                            if (VD->isConstexpr()) {
                                auto InitExpr = VD->getInit();
                                if (InitExpr && llvm::isa<clang::BinaryOperator>(InitExpr)) {
                                    std::string StructName = "MutatedStruct";
                                    std::string MemberName = "member";
                                    std::string PtrName = "ptr";
                                    std::string NewCode = "struct " + StructName + " { int " + MemberName + "; };\n";
                                    NewCode += StructName + " *" + PtrName + " = nullptr;\n";
                                    NewCode += "constexpr int " + VD->getNameAsString() + " = " + PtrName + "->" + MemberName + " + 20;";

                                    Rewrite.ReplaceText(VD->getSourceRange(), NewCode);
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_526::MutatorASTConsumer_526::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isConstexpr()).bind("ConstexprFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}