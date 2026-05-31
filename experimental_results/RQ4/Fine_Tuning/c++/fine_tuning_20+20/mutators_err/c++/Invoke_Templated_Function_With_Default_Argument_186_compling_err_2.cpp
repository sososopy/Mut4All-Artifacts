//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invoke_Templated_Function_With_Default_Argument_186
 */ 
class MutatorFrontendAction_186 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(186)

private:
    class MutatorASTConsumer_186 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_186(Rewriter &R) : TheRewriter(R) {}
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
        const clang::FunctionDecl *cur_func;
    };
};

//source file
#include "../include/invoke_templated_function_with_default_argument_186.h"

// ========================================================================================================
#define MUT186_OUTPUT 1

void MutatorFrontendAction_186::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isCompleteDefinition())
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (auto func = dyn_cast<FunctionDecl>(decl)) {
          if (func->isTemplated() && func->getNumParams() == 1) {
            auto params = func->parameters();
            for (auto param : params) {
              if (param->hasDefaultArg())
                cur_func = func;
            }
          }
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>( "Structs")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isCompleteDefinition())
        return;
      if (DL->isStruct() == false)
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (auto ts = dyn_cast<ClassTemplateSpecializationDecl>(decl)) {
          if (ts->getNameAsString() == "Mut_186") {
            llvm::outs() << "mut186 find specialization\n";
            auto args = ts->getTemplateArgs();
            if (args.size() != 1)
              return;
            if (args[0].getKind() != TemplateArgument::ArgKind::Integral)
              return;
            llvm::outs() << "mut186 find specialization with integral\n";
            if (cur_func == nullptr)
              return;
            llvm::outs() << "mut186 find function\n";
            auto DLname = DL->getNameAsString();
            auto funcname = cur_func->getNameAsString();
            string ins = DLname + "::" + funcname + "<>";
            llvm::outs() << ins << '\n';
            Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut186*/" + ins);
          }
        }
      }
    }
}

void MutatorFrontendAction_186::MutatorASTConsumer_186::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl(isTemplateInstantiation()).bind("Classes");
    auto struct_matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.matchAST(Context);
}