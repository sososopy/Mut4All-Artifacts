//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_237
 */ 

class MutatorFrontendAction_237 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(237)
private:
    class MutatorASTConsumer_237 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_237(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_instantiation_237.h"

// ========================================================================================================
#define MUT237_OUTPUT 1

void MutatorFrontendAction_237::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      if (TD->isThisDeclarationADefinition()) {
        //Get the source code text of target node
        auto templateDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                                    TD->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string recursiveTemplate = R"(
        template<int N>
        struct Recursive {
            static const int value = N + Recursive<N - 1>::value;
        };

        template<>
        struct Recursive<0> {
            static const int value = 0;
        };

        /*mut237*/ Recursive<5> recInstance;
        )";

        templateDecl.insert(templateDecl.find("{") + 1, recursiveTemplate);

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), templateDecl);
      }
    }
}
  
void MutatorFrontendAction_237::MutatorASTConsumer_237::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}