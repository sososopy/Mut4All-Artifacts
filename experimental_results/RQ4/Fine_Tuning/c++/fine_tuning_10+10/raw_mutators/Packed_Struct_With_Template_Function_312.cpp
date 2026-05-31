//header file
#pragma once
#include "Mutator_base.h"

/**
 * Packed_Struct_With_Template_Function_312
 */ 
class MutatorFrontendAction_312 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(312)

private:
    class MutatorASTConsumer_312 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_312(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_312.h"

// ========================================================================================================
#define MUT312_OUTPUT 1

void MutatorFrontendAction_312::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *StructDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("packedStruct")) {
      if (!StructDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                     StructDecl->getLocation()))
        return;

      for (auto *D : StructDecl->decls()) {
          if (auto *FuncTemplate = llvm::dyn_cast<clang::FunctionTemplateDecl>(D)) {
              if (auto *Func = FuncTemplate->getTemplatedDecl()) {
                  if (Func->hasBody()) {
                      auto body = Func->getBody();
                      std::string newBody = "{\nT* optPtr = (T*)&options;\noptPtr->someMethod();\n}";
                      Rewrite.ReplaceText(body->getSourceRange(), newBody);
                      return;
                  }
              }
          }
      }

      std::string newTemplateFunction = R"(
      template <typename T> void processOptions() {
          T* optPtr = (T*)&options;
          optPtr->someMethod();
      }
      )";
      Rewrite.InsertTextAfterToken(StructDecl->getRBraceLoc(), newTemplateFunction);
    }
}
  
void MutatorFrontendAction_312::MutatorASTConsumer_312::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct(), hasAttr(clang::attr::Packed)).bind("packedStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}