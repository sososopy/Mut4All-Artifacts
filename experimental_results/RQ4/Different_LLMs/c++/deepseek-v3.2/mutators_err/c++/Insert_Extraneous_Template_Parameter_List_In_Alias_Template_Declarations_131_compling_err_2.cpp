//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Extraneous_Template_Parameter_List_In_Alias_Template_Declarations_131
 */ 
class MutatorFrontendAction_131 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(131)

private:
    class MutatorASTConsumer_131 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_131(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Insert_Extraneous_Template_Parameter_List_In_Alias_Template_Declarations_131.h"

// ========================================================================================================
#define MUT131_OUTPUT 1

void MutatorFrontendAction_131::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AD = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("AliasTemplateDecl")) {
      //Filter nodes in header files
      if (!AD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AD->getLocation()))
        return;
      //Get the source code text of target node
      auto *TAD = AD->getTemplatedDecl();
      if (!TAD) return;
      
      SourceRange FullRange = AD->getSourceRange();
      if (FullRange.isInvalid()) return;
      
      SourceLocation TemplateStart = AD->getTemplateParameters()->getLocStart();
      SourceLocation UsingStart = TAD->getBeginLoc();
      
      if (TemplateStart.isInvalid() || UsingStart.isInvalid()) return;
      
      //Perform mutation on the source code text by applying string replacement
      std::string Insertion = "\ntemplate <typename K>\n";
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(TemplateStart, Insertion);
    }
}
  
void MutatorFrontendAction_131::MutatorASTConsumer_131::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("AliasTemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}