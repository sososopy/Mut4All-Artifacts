//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Union_Member_Reordering_263
 */ 
class MutatorFrontendAction_263 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(263)

private:
    class MutatorASTConsumer_263 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_263(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/struct_union_member_reordering_263.h"

// ========================================================================================================
#define MUT263_OUTPUT 1

void MutatorFrontendAction_263::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructUnion")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isCompleteDefinition())
        return;
      if (!DL->isStruct() && !DL->isUnion())
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto fields = DL->fields();
      vector<string> field_names;
      for (auto field : fields) {
        field_names.push_back(stringutils::rangetoStr(
            *(Result.SourceManager), field->getSourceRange()));
      }
      std::shuffle(field_names.begin(), field_names.end(),
                   std::mt19937(std::random_device()()));
      string new_content = "";
      for (auto field_name : field_names) {
        new_content += field_name + ";\n";
      }
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << new_content << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DL->getBeginLoc(), DL->getEndLoc(), new_content);
    }
}
  
void MutatorFrontendAction_263::MutatorASTConsumer_263::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("StructUnion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}