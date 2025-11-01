//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Macro_With_Expression_216
 */ 
class MutatorFrontendAction_216 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(216)

private:
    class MutatorASTConsumer_216 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_216(Rewriter &R) : TheRewriter(R) {}
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
        const MacroInfo *TargetMacroInfo = nullptr;
        const Expr *StructMemberExpr = nullptr;
    };
};

//source file
#include "../include/Replace_Macro_With_Expression_216.h"

// ========================================================================================================
#define MUT216_OUTPUT 1

void MutatorFrontendAction_216::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("macroCall")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CE->getBeginLoc()))
            return;

        if (auto *ME = Result.Nodes.getNodeAs<clang::MemberExpr>("memberExpr")) {
            StructMemberExpr = ME;
        }

        if (StructMemberExpr && TargetMacroInfo) {
            std::string macroReplacement;
            llvm::raw_string_ostream stream(macroReplacement);
            StructMemberExpr->printPretty(stream, nullptr, Result.Context->getPrintingPolicy());

            std::string macroBody = TargetMacroInfo->getBody().str();
            size_t pos = macroBody.find("x");
            if (pos != std::string::npos) {
                macroBody.replace(pos, 1, stream.str());
            }
            
            pos = macroBody.find("y");
            if (pos != std::string::npos) {
                macroBody.replace(pos, 1, "5");
            }

            Rewrite.ReplaceText(CE->getSourceRange(), macroBody);
        }
    }
}

void MutatorFrontendAction_216::MutatorASTConsumer_216::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    auto macroMatcher = callExpr(callee(functionDecl(hasName("max"))), 
                                 hasArgument(1, memberExpr().bind("memberExpr")))
                        .bind("macroCall");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(macroMatcher, &callback);
    matchFinder.matchAST(Context);
}