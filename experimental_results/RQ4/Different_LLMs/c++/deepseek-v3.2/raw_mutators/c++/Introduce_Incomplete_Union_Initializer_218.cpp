//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Incomplete_Union_Initializer_218
 */ 
class MutatorFrontendAction_218 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(218)
private:
    class MutatorASTConsumer_218 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_218(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> unionMembers;
    };
};

//source file
#include "../include/Introduce_Incomplete_Union_Initializer_218.h"

// ========================================================================================================
#define MUT218_OUTPUT 1

void MutatorFrontendAction_218::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UD = Result.Nodes.getNodeAs<clang::UnionDecl>("UnionDecl")) {
      //Filter nodes in header files
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;
      //Record union member declarations
      for (auto member : UD->decls()) {
        if (auto *VD = dyn_cast<VarDecl>(member)) {
          unionMembers.push_back(VD);
        }
      }
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if this VarDecl is a union member
      bool isUnionMember = false;
      for (auto member : unionMembers) {
        if (member == VD) {
          isUnionMember = true;
          break;
        }
      }
      if (!isUnionMember) return;
      
      //Get the source code text of target node
      auto initRange = VD->getInitRange();
      std::string memberText = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      if (initRange.isValid()) {
        // Member has an initializer, replace it with incomplete initializer
        std::string initText = stringutils::rangetoStr(*(Result.SourceManager), initRange);
        size_t pos = memberText.find(initText);
        if (pos != std::string::npos) {
          memberText.replace(pos, initText.length(), "= ;");
        }
      } else {
        // Member has no initializer, add incomplete initializer
        size_t pos = memberText.find(';');
        if (pos != std::string::npos) {
          memberText.insert(pos, " = ;");
        }
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), memberText);
    }
}
  
void MutatorFrontendAction_218::MutatorASTConsumer_218::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher unionMatcher = recordDecl(isUnion()).bind("UnionDecl");
    DeclarationMatcher varDeclMatcher = varDecl().bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(unionMatcher, &callback);
    matchFinder.addMatcher(varDeclMatcher, &callback);
    matchFinder.matchAST(Context);
}