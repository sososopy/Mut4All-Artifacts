//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_266
 */ 
class MutatorFrontendAction_266 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(266)

private:
    class MutatorASTConsumer_266 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_266(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_instantiation_266.h"

// ========================================================================================================
#define MUT266_OUTPUT 1

void MutatorFrontendAction_266::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FD->getBody()->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string recursiveTemplate = R"(
template<int N>
struct RecursiveTemplate {
    static void call() {
        RecursiveTemplate<N-1>::call();
    }
};

template<>
struct RecursiveTemplate<0> {
    static void call() {}
};

RecursiveTemplate<100>::call();
)";

        functionBody.insert(functionBody.find("{") + 1, "\n/*mut266*/" + recursiveTemplate);

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBody);
      }
    }
}
  
void MutatorFrontendAction_266::MutatorASTConsumer_266::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasBody()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}