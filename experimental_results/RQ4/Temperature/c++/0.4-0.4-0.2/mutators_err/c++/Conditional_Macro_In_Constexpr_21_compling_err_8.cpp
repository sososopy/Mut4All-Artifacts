//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * conditional_macro_in_constexpr_21
 */ 
class MutatorFrontendAction_21 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(21)

private:
    class MutatorASTConsumer_21 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_21(Rewriter &R) : TheRewriter(R) {}
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
        const clang::MacroInfo *macroInfo = nullptr;
        const clang::VarDecl *constexprVar = nullptr;
    };
};

//source file
#include "../include/conditional_macro_in_constexpr_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MI = Result.Nodes.getNodeAs<clang::MacroInfo>("macroInfo")) {
        if (!MI || !Result.Context->getSourceManager().isWrittenInMainFile(
                       MI->getDefinitionLoc()))
            return;
        macroInfo = MI;
    }
    
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("constexprVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;
        if (!VD->isConstexpr())
            return;

        constexprVar = VD;
        if (macroInfo && constexprVar) {
            auto macroUsage = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
            // Perform mutation on the source code text by applying string replacement
            std::string mutatedMacroUsage = macroUsage;
            if (mutatedMacroUsage.find("max(") != std::string::npos) {
                mutatedMacroUsage.replace(mutatedMacroUsage.find("max("), 4, "max(20, st->value)");
            }
            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), mutatedMacroUsage);
        }
    }
}

void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    auto macroMatcher = macroDefinition().bind("macroInfo");
    auto constexprMatcher = varDecl(hasInitializer(expr(anyOf(hasDescendant(callExpr(callee(functionDecl(hasName("max"))))), hasDescendant(memberExpr()))))).bind("constexprVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(macroMatcher, &callback);
    matchFinder.addMatcher(constexprMatcher, &callback);
    matchFinder.matchAST(Context);
}