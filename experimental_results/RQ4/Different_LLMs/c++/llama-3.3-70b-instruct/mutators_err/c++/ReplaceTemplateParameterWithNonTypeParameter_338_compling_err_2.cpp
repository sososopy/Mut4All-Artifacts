//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * ReplaceTemplateParameterWithNonTypeParameter_338
 */ 
class MutatorFrontendAction_338 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(338)

private:
    class MutatorASTConsumer_338 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_338(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceTemplateParameterWithNonTypeParameter_338.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT338_OUTPUT 1

void MutatorFrontendAction_338::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace 'class' with 'int' or 'void*' in template parameters
      std::string mutatedDeclaration = declaration;
      size_t pos = mutatedDeclaration.find("class");
      if (pos!= std::string::npos) {
        mutatedDeclaration.replace(pos, 5, "int"); // Replace 'class' with 'int'
      } else {
        pos = mutatedDeclaration.find("typename");
        if (pos!= std::string::npos) {
          mutatedDeclaration.replace(pos, 8, "void*"); // Replace 'typename' with 'void*'
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_338::MutatorASTConsumer_338::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}