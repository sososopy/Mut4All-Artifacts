//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Replace_Concept_Template_Parameter_With_Non_Template_Parameter_103
 */ 
class MutatorFrontendAction_103 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(103)

private:
    class MutatorASTConsumer_103 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_103(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Concept_Template_Parameter_With_Non_Template_Parameter_103.h"

// ========================================================================================================
#define MUT103_OUTPUT 1

void MutatorFrontendAction_103::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concept")) {
      //Filter nodes in header files
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 CT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace template <class> typename Op with typename Op
      size_t pos = declaration.find("template <class> typename");
      if (pos != string::npos) {
        declaration.replace(pos, 24, "typename");
      }
      // Replace template template parameter with non-template type parameter
      pos = declaration.find("template <typename");
      if (pos != string::npos) {
        declaration.replace(pos, 17, "typename");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_103::MutatorASTConsumer_103::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = conceptDecl(isDefinition()).bind("Concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}