//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Member_With_Function_Ref_159
 */ 
class MutatorFrontendAction_159 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(159)

private:
    class MutatorASTConsumer_159 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_159(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Static_Member_With_Function_Ref_159.h"

// ========================================================================================================
#define MUT159_OUTPUT 1

void MutatorFrontendAction_159::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("staticField")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        SourceManager &SM = *Result.SourceManager;
        SourceRange range = FD->getSourceRange();
        std::string originalCode = stringutils::rangetoStr(SM, range);

        size_t staticPos = originalCode.find("static ");
        if (staticPos != 0)
            return;

        std::string rest = originalCode.substr(7);
        size_t semiPos = rest.find(';');
        if (semiPos == std::string::npos)
            return;

        std::string declPart = rest.substr(0, semiPos);
        size_t lastSpace = declPart.rfind(' ');
        if (lastSpace == std::string::npos)
            return;

        std::string typePart = declPart.substr(0, lastSpace);
        std::string namePart = declPart.substr(lastSpace + 1);

        std::string newDecl = typePart + " (*" + namePart + ")()";
        std::string newCode = "static " + newDecl + ";";

        Rewrite.ReplaceText(range, newCode);
    }
}

void MutatorFrontendAction_159::MutatorASTConsumer_159::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(isStatic(), inRecord()).bind("staticField");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}