//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Function_Argument_Manipulation_439
 */ 
class MutatorFrontendAction_439 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(439)

private:
    class MutatorASTConsumer_439 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_439(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Function_Argument_Manipulation_439.h"

// ========================================================================================================
#define MUT439_OUTPUT 1

void MutatorFrontendAction_439::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto template_code = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << template_code << '\n';
      if (template_code.find("int") != string::npos)
        template_code.replace(template_code.find("int"), 3, "class");
      else if (template_code.find("typename") != string::npos)
        template_code.replace(template_code.find("typename"), 8, "int");
      template_code = "/*mut439*/" + template_code;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), template_code);
    }
}
  
void MutatorFrontendAction_439::MutatorASTConsumer_439::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}