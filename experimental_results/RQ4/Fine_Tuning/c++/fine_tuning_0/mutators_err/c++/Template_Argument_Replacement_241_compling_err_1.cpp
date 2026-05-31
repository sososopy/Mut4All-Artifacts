//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Argument_Replacement_241
 */ 
class MutatorFrontendAction_241 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(241)

private:
    class MutatorASTConsumer_241 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_241(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Argument_Replacement_241.h"

// ========================================================================================================
#define MUT241_OUTPUT 1

void MutatorFrontendAction_241::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateArgument>("TemplateArgs")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto arg = stringutils::rangetoStr(*(Result.SourceManager),
                                         MT->getSourceRange());
      llvm::outs() << arg << '\n';
      if (MT->getKind() != TemplateArgument::Type)
        return;
      auto type = MT->getAsType();
      if (!type->isBuiltinType())
        return;
      //Perform mutation on the source code text by applying string replacement
      string replacement = "Incomplete";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_241::MutatorASTConsumer_241::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateArgument().bind("TemplateArgs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}