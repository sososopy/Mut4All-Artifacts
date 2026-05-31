//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateArgumentTypeMismatch_Mutator_240
 */ 
class MutatorFrontendAction_240 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(240)

private:
    class MutatorASTConsumer_240 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_240(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/TemplateArgumentTypeMismatch_Mutator_240.h"

// ========================================================================================================
#define MUT240_OUTPUT 1

void MutatorFrontendAction_240::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Introduce a type mismatch by changing the template parameter type
      // For example, change 'typename T' to 'typename T&' or 'typename T*'
      std::string mutatedDeclaration = declaration;
      size_t pos = declaration.find("typename");
      if (pos != std::string::npos) {
        mutatedDeclaration.insert(pos + 8, "&"); // Change to reference type
        //mutatedDeclaration.insert(pos + 8, "*"); // Change to pointer type
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_240::MutatorASTConsumer_240::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}