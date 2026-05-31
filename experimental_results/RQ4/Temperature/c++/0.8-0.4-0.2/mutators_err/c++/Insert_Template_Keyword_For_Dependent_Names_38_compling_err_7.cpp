//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Template_Keyword_For_Dependent_Names_38
 */ 
class MutatorFrontendAction_38 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(38)

private:
    class MutatorASTConsumer_38 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_38(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_38.h"

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::DependentNameType>("DependentName")) {
      //Filter nodes in header files
      if (!MT || !Result.SourceManager->isWrittenInMainFile(
                     MT->getLocStart()))
        return;
      //Get the source code text of target node
      if (const auto *NNS = MT->getQualifier()) {
        auto sourceRange = NNS->getSourceRange();
        auto text = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange),
                                         *Result.SourceManager, Result.Context->getLangOpts());
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedText = text.str() + "template " + MT->getIdentifier()->getName().str();
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(sourceRange, mutatedText);
      }
    }
}
  
void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = type().bind("DependentName");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}