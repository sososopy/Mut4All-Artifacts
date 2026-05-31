//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * misplaced_static_member_initialization_49
 */ 
class MutatorFrontendAction_49 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(49)

private:
    class MutatorASTConsumer_49 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_49(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misplaced_static_member_initialization_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::RecordDecl>("anonStruct")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RD->getLocation()))
            return;

        if (!RD->isAnonymousStructOrUnion())
            return;

        std::string structContent = RD->getSourceRange().printToString(*Result.SourceManager);
        std::string staticMember = "static int staticMember = 10; // Incorrectly initialized static member within an anonymous struct\n";
        
        if (structContent.find("static int staticMember") == std::string::npos) {
            size_t insertPos = structContent.find("{") + 1;
            structContent.insert(insertPos, staticMember);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(RD->getSourceRange()), structContent);
        }
    }
}
  
void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(hasAttr<clang::attr::AnonymousStructOrUnion>()).bind("anonStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}