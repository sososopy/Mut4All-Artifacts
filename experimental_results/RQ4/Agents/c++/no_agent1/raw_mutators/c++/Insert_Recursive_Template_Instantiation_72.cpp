//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Recursive_Template_Instantiation_72
 */ 
class MutatorFrontendAction_72 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(72)

private:
    class MutatorASTConsumer_72 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_72(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_recursive_template_instantiation_72.h"

// ========================================================================================================
#define MUT72_OUTPUT 1

void MutatorFrontendAction_72::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      if (TD->isThisDeclarationADefinition()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string recursiveTemplate = R"(
template<int N>
struct RecursiveTemplate {
    static const int value = RecursiveTemplate<N-1>::value;
};

template<>
struct RecursiveTemplate<0> {
    static const int value = 0;
};
)";
        declaration += recursiveTemplate;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_72::MutatorASTConsumer_72::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}