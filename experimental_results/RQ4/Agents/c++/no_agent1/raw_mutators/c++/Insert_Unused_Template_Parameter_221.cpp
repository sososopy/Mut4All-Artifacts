//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Parameter_221
 */ 
class MutatorFrontendAction_221 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(221)

private:
    class MutatorASTConsumer_221 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_221(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unused_template_parameter_221.h"

// ========================================================================================================
#define MUT221_OUTPUT 1

void MutatorFrontendAction_221::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation())
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string templateParam = "template<typename UnusedParam>\n";
      declaration.insert(0, templateParam);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_221::MutatorASTConsumer_221::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(unless(isTemplateInstantiation())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}