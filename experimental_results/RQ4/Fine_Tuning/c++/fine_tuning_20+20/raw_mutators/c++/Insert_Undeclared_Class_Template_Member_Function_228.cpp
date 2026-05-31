//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Undeclared_Class_Template_Member_Function_228
 */ 
class MutatorFrontendAction_228 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(228)

private:
    class MutatorASTConsumer_228 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_228(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<string> class_names;
    };
};

//source file
#include "../include/Insert_Undeclared_Class_Template_Member_Function_228.h"

// ========================================================================================================
#define MUT228_OUTPUT 1

void MutatorFrontendAction_228::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto name = MT->getNameAsString();
      llvm::outs() << name << '\n';
      class_names.push_back(name);
    } else if (auto *CL =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if (CL->isCompleteDefinition() == false)
        return;
      if (CL->isLambda())
        return;
      auto name = CL->getNameAsString();
      if (name == "")
        return;
      if (CL->isStruct() == false && CL->isClass() == false)
        return;
      llvm::outs() << name << '\n';
      class_names.push_back(name);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DL")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          return;
        }
      }
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto DLname = DL->getNameAsString();
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DLname == "")
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      llvm::outs() << DLname << '\n';
      if (DL->isStruct() == false && DL