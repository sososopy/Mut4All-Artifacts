//header file
#pragma once
#include "Mutator_base.h"

/**
 * add_vararg_friend_operator_41
 */ 
class MutatorFrontendAction_41 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(41)

private:
    class MutatorASTConsumer_41 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_41(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/add_vararg_friend_operator_41.h"

// ========================================================================================================
#define MUT41_OUTPUT 1

void MutatorFrontendAction_41::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SR = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
        if (!SR || !Result.Context->getSourceManager().isWrittenInMainFile(SR->getLocation()))
            return;

        if (SR->isStruct() && SR->hasDefinition()) {
            auto sourceRange = SR->getSourceRange();
            auto sourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), 
                                                   *Result.SourceManager, 
                                                   Result.Context->getLangOpts());
            std::string newFriendFunction = "\nfriend void operator,(MyStruct, ...); // New variadic friend function\n";
            size_t insertPosition = sourceText.rfind('}');
            if (insertPosition != std::string::npos) {
                sourceText.insert(insertPosition, newFriendFunction);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
            }
        }
    }
}

void MutatorFrontendAction_41::MutatorASTConsumer_41::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct(), hasDefinition(), has(friendDecl())).bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}