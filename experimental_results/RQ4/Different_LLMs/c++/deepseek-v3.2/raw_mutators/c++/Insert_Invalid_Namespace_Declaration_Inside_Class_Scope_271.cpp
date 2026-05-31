//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Namespace_Declaration_Inside_Class_Scope_271
 */ 
class MutatorFrontendAction_271 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(271)

private:
    class MutatorASTConsumer_271 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_271(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> classes;
    };
};

//source file
#include "../include/Insert_Invalid_Namespace_Declaration_Inside_Class_Scope_271.h"

// ========================================================================================================
#define MUT271_OUTPUT 1

void MutatorFrontendAction_271::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (!MT->isCompleteDefinition())
        return;
      //(optional)Record the node information to be used in the mutation process
      classes.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TargetClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      //Get the source code text of target node
      std::string name_to_insert;
      bool found_member = false;
      for (auto decl : MT->decls()) {
        if (auto field = dyn_cast<FieldDecl>(decl)) {
          name_to_insert = field->getNameAsString();
          found_member = true;
          break;
        }
        else if (auto method = dyn_cast<CXXMethodDecl>(decl)) {
          name_to_insert = method->getNameAsString();
          found_member = true;
          break;
        }
      }
      if (!found_member) {
        std::vector<std::string> fallback_names = {"memmove", "memcpy", "malloc", "free", "printf"};
        int idx = getrandom::getRandomIndex(fallback_names.size() - 1);
        name_to_insert = fallback_names[idx];
      }
      //Perform mutation on the source code text by applying string replacement
      std::string insertion = "\n/*mut271*/namespace using ::" + name_to_insert + ";";
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(MT->getEndLoc(), insertion);
    }
}
  
void MutatorFrontendAction_271::MutatorASTConsumer_271::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    DeclarationMatcher target_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("TargetClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(target_matcher, &callback);
    matchFinder.matchAST(Context);
}