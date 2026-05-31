//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Struct_Member_Function_With_Concept_481
 */ 
class MutatorFrontendAction_481 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(481)
    
    private:
      class MutatorASTConsumer_481 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_481(Rewriter &R) : TheRewriter(R) {}
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
          std::set<string> visited;
      };
    };

//source file
#include "../include/insert_struct_member_function_with_concept_481"

// ========================================================================================================
#define MUT481_OUTPUT 1

void MutatorFrontendAction_481::Callback::run(const MatchFinder::MatchResult &Result) {
  if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDecl")) {
    if (!recordDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
      return;
    if (!recordDecl->isStruct())
      return;
    if (!recordDecl->isCompleteDefinition())
      return;
    auto recordName = recordDecl->getNameAsString();
    if (recordName == "")
      return;
    if (visited.find(recordName) == visited.end()) {
      visited.insert(recordName);
      llvm::errs() << recordName << "\n";
      string insText = "template <typename T> requires std::integral<T> void mut_481(T){}\n";
      SourceLocation insLoc = recordDecl->getEndLoc();
      Rewrite.InsertTextAfterToken(insLoc, ";\n/*mut481*/"+insText);
    }
  } else if (const VarDecl *varDecl = Result.Nodes.getNodeAs<VarDecl>("varDecl")) {
    if (!varDecl)
      return;
    if (!Result.Context->getSourceManager().isWrittenInMainFile(varDecl->getBeginLoc()))
      return;
    auto type = varDecl->getType();
    if (!type->isStructureType())
      return;
    auto typeName = type.getAsString();
    if (visited.find(typeName) == visited.end())
      return;
    auto varName = varDecl->getNameAsString();
    auto insText = "std::function<void(int)> mut_481 = std::bind(&" + typeName +
                   "::mut_481<int>, " + varName + ", std::placeholders::_1);\n";
    llvm::errs() << insText << "\n";
    auto endLoc = varDecl->getEndLoc();
    Rewrite.InsertTextAfterToken(endLoc, ";\n/*mut481*/"+insText);
  }
}

void MutatorFrontendAction_481::MutatorASTConsumer_481::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("recordDecl");
    auto var_matcher = varDecl().bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.matchAST(Context);
}