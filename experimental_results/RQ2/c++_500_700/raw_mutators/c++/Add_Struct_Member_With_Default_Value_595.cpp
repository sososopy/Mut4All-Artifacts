```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * add_struct_member_with_default_value_595
 */ 
class MutatorFrontendAction_595 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(595)

private:
    class MutatorASTConsumer_595 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_595(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/add_struct_member_with_default_value_595.h"

// ========================================================================================================
#define MUT595_OUTPUT 1

void MutatorFrontendAction_595::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructUnion")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isCompleteDefinition() || (!RD->isStruct() && !RD->isUnion()))
        return;
      
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager), RD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string newMember = "\n    float z = 3.14f; // Newly added member with default value";
      if (content.rfind('}') != std::string::npos)
        content.insert(content.rfind('}'), newMember);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RD->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_595::MutatorASTConsumer_595::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("StructUnion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```