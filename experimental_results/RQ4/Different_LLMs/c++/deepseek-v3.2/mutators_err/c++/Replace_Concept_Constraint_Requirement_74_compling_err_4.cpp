//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_Constraint_Requirement_74
 */ 
class MutatorFrontendAction_74 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(74)

private:
    class MutatorASTConsumer_74 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_74(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT74_OUTPUT 1

void MutatorFrontendAction_74::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concept")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto conceptSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //(optional)Record the node information to be used in the mutation process
      //Perform mutation on the source code text by applying string replacement
      size_t requiresPos = conceptSource.find("requires");
      if (requiresPos == std::string::npos)
        return;
      size_t conditionStart = conceptSource.find("sizeof", requiresPos);
      if (conditionStart == std::string::npos)
        return;
      size_t opStart = conceptSource.find_first_of("><=!", conditionStart);
      if (opStart == std::string::npos)
        return;
      size_t opEnd = opStart;
      if (conceptSource[opEnd] == '=') {
        opEnd++;
      } else if (opEnd + 1 < conceptSource.size() && conceptSource[opEnd] == '!' && conceptSource[opEnd + 1] == '=') {
        opEnd += 2;
      } else {
        opEnd++;
      }
      std::string originalOp = conceptSource.substr(opStart, opEnd - opStart);
      std::string newOp;
      if (originalOp == ">")
        newOp = "<=";
      else if (originalOp == ">=")
        newOp = "<";
      else if (originalOp == "<")
        newOp = ">=";
      else if (originalOp == "<=")
        newOp = ">";
      else if (originalOp == "==")
        newOp = "!=";
      else if (originalOp == "!=")
        newOp = "==";
      else
        return;
      conceptSource.replace(opStart, opEnd - opStart, newOp);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), conceptSource);
    }
}
  
void MutatorFrontendAction_74::MutatorASTConsumer_74::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the1 target AST node for mutation.
    DeclarationMatcher matcher = conceptDecl().bind("Concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}