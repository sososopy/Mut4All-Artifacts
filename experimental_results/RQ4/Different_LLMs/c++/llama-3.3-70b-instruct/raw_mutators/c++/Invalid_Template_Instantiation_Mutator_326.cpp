//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Template_Instantiation_Mutator_326
 */ 
class MutatorFrontendAction_326 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(326)

private:
    class MutatorASTConsumer_326 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_326(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Template_Instantiation_Mutator_326.h"

// ========================================================================================================
#define MUT326_OUTPUT 1

void MutatorFrontendAction_326::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Change template parameter type
      size_t pos = declaration.find("typename");
      if (pos != std::string::npos) {
        declaration.replace(pos, 8, "int");
      }
      // Add extra template parameter
      pos = declaration.find(">");
      if (pos != std::string::npos) {
        declaration.insert(pos, ", typename U");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_326::MutatorASTConsumer_326::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}