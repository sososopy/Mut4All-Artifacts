//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Closing_Delimiter_192
 */ 
class MutatorFrontendAction_192 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(192)

private:
    class MutatorASTConsumer_192 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_192(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Remove_Closing_Delimiter_192.h"

// ========================================================================================================
#define MUT192_OUTPUT 1

void MutatorFrontendAction_192::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *S = Result.Nodes.getNodeAs<clang::Stmt>("stmt")) {
        // Filter nodes in header files
        if (!Result.Context->getSourceManager().isWrittenInMainFile(S->getBeginLoc()))
            return;
        
        if (auto *FD = dyn_cast<clang::FunctionDecl>(S)) {
            if (!FD->hasBody())
                return;
            auto body = FD->getBody();
            if (!body)
                return;
            if (auto *CS = dyn_cast<clang::CompoundStmt>(body)) {
                SourceRange range = CS->getSourceRange();
                auto SM = Result.SourceManager;
                std::string originalText = stringutils::rangetoStr(*SM, range);
                if (originalText.empty())
                    return;
                std::string mutatedText = originalText.substr(0, originalText.size() - 1);
                Rewrite.ReplaceText(range, mutatedText);
            }
        } else if (auto *IS = dyn_cast<clang::IfStmt>(S)) {
            auto thenStmt = IS->getThen();
            if (auto *CS = dyn_cast<clang::CompoundStmt>(thenStmt)) {
                SourceRange range = CS->getSourceRange();
                auto SM = Result.SourceManager;
                std::string originalText = stringutils::rangetoStr(*SM, range);
                if (originalText.empty())
                    return;
                std::string mutatedText = originalText.substr(0, originalText.size() - 1);
                Rewrite.ReplaceText(range, mutatedText);
            }
        } else if (auto *FS = dyn_cast<clang::ForStmt>(S)) {
            auto body = FS->getBody();
            if (auto *CS = dyn_cast<clang::CompoundStmt>(body)) {
                SourceRange range = CS->getSourceRange();
                auto SM = Result.SourceManager;
                std::string originalText = stringutils::rangetoStr(*SM, range);
                if (originalText.empty())
                    return;
                std::string mutatedText = originalText.substr(0, originalText.size() - 1);
                Rewrite.ReplaceText(range, mutatedText);
            }
        } else if (auto *WS = dyn_cast<clang::WhileStmt>(S)) {
            auto body = WS->getBody();
            if (auto *CS = dyn_cast<clang::CompoundStmt>(body)) {
                SourceRange range = CS->getSourceRange();
                auto SM = Result.SourceManager;
                std::string originalText = stringutils::rangetoStr(*SM, range);
                if (originalText.empty())
                    return;
                std::string mutatedText = originalText.substr(0, originalText.size() - 1);
                Rewrite.ReplaceText(range, mutatedText);
            }
        } else if (auto *DS = dyn_cast<clang::DoStmt>(S)) {
            auto body = DS->getBody();
            if (auto *CS = dyn_cast<clang::CompoundStmt>(body)) {
                SourceRange range = CS->getSourceRange();
                auto SM = Result.SourceManager;
                std::string originalText = stringutils::rangetoStr(*SM, range);
                if (originalText.empty())
                    return;
                std::string mutatedText = originalText.substr(0, originalText.size() - 1);
                Rewrite.ReplaceText(range, mutatedText);
            }
        } else if (auto *SS = dyn_cast<clang::SwitchStmt>(S)) {
            auto body = SS->getBody();
            if (auto *CS = dyn_cast<clang::CompoundStmt>(body)) {
                SourceRange range = CS->getSourceRange();
                auto SM = Result.SourceManager;
                std::string originalText = stringutils::rangetoStr(*SM, range);
                if (originalText.empty())
                    return;
                std::string mutatedText = originalText.substr(0, originalText.size() - 1);
                Rewrite.ReplaceText(range, mutatedText);
            }
        }
    }
}
  
void MutatorFrontendAction_192::MutatorASTConsumer_192::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = 
        functionDecl(hasBody(compoundStmt())).bind("stmt") ||
        ifStmt(hasThenStmt(compoundStmt())).bind("stmt") ||
        forStmt(hasBody(compoundStmt())).bind("stmt") ||
        whileStmt(hasBody(compoundStmt())).bind("stmt") ||
        doStmt(hasBody(compoundStmt())).bind("stmt") ||
        switchStmt(hasBody(compoundStmt())).bind("stmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}