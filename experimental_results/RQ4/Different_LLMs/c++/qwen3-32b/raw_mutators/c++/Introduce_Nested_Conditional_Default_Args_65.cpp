//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Nested_Conditional_Default_Args_65
 */ 
class MutatorFrontendAction_65 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(65)

private:
    class MutatorASTConsumer_65 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_65(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Introduce_Nested_Conditional_Default_Args_65.h"

// ========================================================================================================
#define MUT65_OUTPUT 1

void MutatorFrontendAction_65::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Param = Result.Nodes.getNodeAs<clang::ParmVarDecl>("param")) {
        if (!Param || !Result.Context->getSourceManager().isWrittenInMainFile(Param->getLocation()))
            return;

        auto *FuncTemplate = Param->getFunctionScopeDecl()->getDescribedFunctionTemplate();
        if (!FuncTemplate)
            return;

        static int varCounter = 0;
        std::string varName = "existingVar_" + std::to_string(varCounter++);
        std::string varDecl = "int " + varName + " = 0; // mutator-generated\n";

        SourceLocation funcLoc = FuncTemplate->getLocation();
        Rewrite.InsertTextBefore(funcLoc, varDecl);

        std::string newDefaultArg = "[" + varName + "]() { "
            "if (true) { "
            "   if (false) { "
            "       return 42; "
            "   } else { "
            "       return " + varName + "; "
            "   } "
            "} else { "
            "   return 0; "
            "} "
            "}()";

        SourceRange paramRange = Param->getDefaultArgRange();
        Rewrite.ReplaceText(paramRange, newDefaultArg);
    }
}

void MutatorFrontendAction_65::MutatorASTConsumer_65::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        hasDescendant(parmVarDecl(hasDefaultArg()).bind("param"))
    ).bind("funcTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}