//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Decl.h"

/**
 * Replace_Export_Module_With_Header_Include_72
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
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Export_Module_With_Header_Include_72.h"

// ========================================================================================================
#define MUT72_OUTPUT 1

void MutatorFrontendAction_72::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ExportDecl>("ExportDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedDeclaration = "#include <" + std::string(MT->getName()) + ".h>\n";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_72::MutatorASTConsumer_72::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::decl(clang::ast_matchers::hasDeclaration(clang::ast_matchers::translationUnitDecl())).bind("ExportDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}