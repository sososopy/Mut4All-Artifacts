//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Union_With_Nontrivial_Class_In_Template_8
 */ 
class MutatorFrontendAction_8 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(8)

private:
    class MutatorASTConsumer_8 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_8(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Union_With_Nontrivial_Class_In_Template_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      //Get the source code text of target node
      auto name = MT->getNameAsString();
      llvm::outs() << name << '\n';
      auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      definition.insert(definition.rfind("}"),
                        "\nunion{class NonTrivial{NonTrivial(){} };};\n");
      definition += name + "<int> mut_8;";
      llvm::outs() << definition << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), definition);
    }
}
  
void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}