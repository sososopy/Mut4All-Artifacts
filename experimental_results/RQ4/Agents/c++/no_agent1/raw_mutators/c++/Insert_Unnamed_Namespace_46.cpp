//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unnamed_Namespace_46
 */ 
class MutatorFrontendAction_46 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(46)

private:
    class MutatorASTConsumer_46 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_46(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unnamed_namespace_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionText = stringutils::rangetoStr(*(Result.SourceManager),
                                                        FD->getSourceRange());

            std::string unnamedNamespace = "namespace {\n/*mut46*/\n";
            unnamedNamespace += functionText;
            unnamedNamespace += "\n}\n";

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), unnamedNamespace);
        }
    }
}

void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}