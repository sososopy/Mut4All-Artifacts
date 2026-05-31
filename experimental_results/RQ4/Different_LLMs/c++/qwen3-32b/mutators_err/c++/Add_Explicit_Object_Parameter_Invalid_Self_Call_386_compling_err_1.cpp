//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Explicit_Object_Parameter_Invalid_Self_Call_386
 */ 
class MutatorFrontendAction_386 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(386)
private:
    class MutatorASTConsumer_386 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_386(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Explicit_Object_Parameter_Invalid_Self_Call_386.h"

// ========================================================================================================
#define MUT386_OUTPUT 1

void MutatorFrontendAction_386::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getBeginLoc()))
            return;
        
        SourceManager &SM = *Result.SourceManager;
        SourceLocation start = lambda->getBeginLoc();
        SourceLocation end = lambda->getEndLoc();
        std::string lambdaText = stringutils::rangetoStr(SM, CharSourceRange::getTokenRange(start, end));
        
        size_t paramStart = lambdaText.find('(');
        size_t paramEnd = lambdaText.find(')');
        if (paramStart == std::string::npos || paramEnd == std::string::npos)
            return;
        
        std::string params = lambdaText.substr(paramStart + 1, paramEnd - paramStart - 1);
        std::string newParams = "this auto& self, " + params;
        lambdaText.replace(paramStart + 1, paramEnd - paramStart - 1, newParams);
        
        size_t arrowPos = paramEnd;
        lambdaText.insert(arrowPos, " -> void");
        
        size_t bodyStart = lambdaText.find('{');
        if (bodyStart != std::string::npos) {
            lambdaText.insert(bodyStart + 1, "\n    self(\"unexpected_argument\");\n");
        }
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(start, end), lambdaText);
    }
}

void MutatorFrontendAction_386::MutatorASTConsumer_386::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = lambdaExpr(hasAnyParameter(decl())).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}