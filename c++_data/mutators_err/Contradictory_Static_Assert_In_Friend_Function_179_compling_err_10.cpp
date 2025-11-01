//header file
#pragma once
#include "Mutator_base.h"

/**
 * Contradictory_Static_Assert_In_Friend_Function_179
 */ 
class MutatorFrontendAction_179 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(179)

private:
    class MutatorASTConsumer_179 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_179(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Contradictory_Static_Assert_In_Friend_Function_179.h"

// ========================================================================================================
#define MUT179_OUTPUT 1

void MutatorFrontendAction_179::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FriendFunction")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getFriendObjectKind() != clang::Decl::FOK_None && FD->isTemplateInstantiation()) {
            std::string functionBody = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());

            size_t pos = functionBody.find("static_assert");
            while (pos != std::string::npos) {
                size_t endPos = functionBody.find(";", pos);
                std::string assertion = functionBody.substr(pos, endPos - pos + 1);
                
                if (assertion.find("std::is_same") != std::string::npos) {
                    std::string contradictoryAssertion = assertion;
                    size_t conditionPos = contradictoryAssertion.find("std::is_same");
                    contradictoryAssertion.insert(conditionPos + 13, "!");
                    functionBody.insert(endPos + 1, "\n" + contradictoryAssertion);
                } else if (assertion.find("sizeof...") != std::string::npos) {
                    std::string contradictoryAssertion = assertion;
                    size_t conditionPos = contradictoryAssertion.find("!=");
                    if (conditionPos != std::string::npos) {
                        contradictoryAssertion.replace(conditionPos, 2, "==");
                    } else {
                        conditionPos = contradictoryAssertion.find("==");
                        contradictoryAssertion.replace(conditionPos, 2, "!=");
                    }
                    functionBody.insert(endPos + 1, "\n" + contradictoryAssertion);
                }

                pos = functionBody.find("static_assert", endPos);
            }

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBody);
        }
    }
}
  
void MutatorFrontendAction_179::MutatorASTConsumer_179::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasFriendDecl(), isTemplateInstantiation()).bind("FriendFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}