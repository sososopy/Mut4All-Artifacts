//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Function_Template_Call_In_Namespace_And_Struct_234
 */ 
class MutatorFrontendAction_234 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(234)

private:
    class MutatorASTConsumer_234 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_234(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        const clang::NamespaceDecl *target_ns;
        const clang::FunctionTemplateDecl *target_ft;
        const clang::CXXRecordDecl *target_st;
    };
};

//source file
#include "../include/insert_function_template_call_in_namespace_and_struct_234.h"

// ========================================================================================================
#define MUT234_OUTPUT 1

void MutatorFrontendAction_234::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespaces")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       NS->getLocation()))
          return;
        target_ns = NS;
      } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                     "FuncTemplates")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FT->getLocation()))
          return;
        target_ft = FT;
      } else if (auto *ST =
                     Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
        if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                       ST->getLocation()))
          return;
        target_st = ST;
      }
}
  
void MutatorFrontendAction_234::MutatorASTConsumer_234::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto ns_matcher = namespaceDecl().bind("Namespaces");
    auto ft_matcher = functionTemplateDecl().bind("FuncTemplates");
    auto st_matcher = cxxRecordDecl(isStruct()).bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(ns_matcher, &callback);
    matchFinder.addMatcher(ft_matcher, &callback);
    matchFinder.addMatcher(st_matcher, &callback);
    matchFinder.matchAST(Context);
    if (callback.target_ft == nullptr) {
      llvm::errs() << "No function template found!\n";
      return;
    }
    auto ft_name = callback.target_ft->getNameAsString();
    llvm::errs() << "Function template: " << ft_name << '\n';
    if (callback.target_ns == nullptr) {
      llvm::errs() << "No namespace found!\n";
      return;
    }
    auto ns_name = callback.target_ns->getNameAsString();
    llvm::errs() << "Namespace: " << ns_name << '\n';
    if (callback.target_st == nullptr) {
      llvm::errs() << "No struct found!\n";
      return;
    }
    auto st_name = callback.target_st->getNameAsString();
    llvm::errs() << "Struct: " << st_name << '\n';
    auto ft_content = stringutils::rangetoStr(TheRewriter.getSourceMgr(),
                                              callback.target_ft->getSourceRange());
    if (ft_content.find("namespace") == string::npos) {
      ft_content = "namespace " + ns_name + "{\n" + ft_content + "\n}\n";
    }
    llvm::errs() << ft_content << '\n';
    TheRewriter.ReplaceText(
        CharSourceRange::getTokenRange(callback.target_ft->getSourceRange()),
        ft_content);
    auto st_content = stringutils::rangetoStr(TheRewriter.getSourceMgr(),
                                              callback.target_st->getSourceRange());
    if (st_content.find("namespace") == string::npos) {
      st_content = "namespace " + ns_name + "{\n" + st_content + "\n}\n";
    }
    if (st_content.rfind('}') != string::npos) {
      st_content.insert(st_content.rfind('}'),
                        "\nvoid test(){\n" + ft_name + "<" + st_name +
                            ">();\n}\n");
    }
    llvm::errs() << st_content << '\n';
    TheRewriter.ReplaceText(
        CharSourceRange::getTokenRange(callback.target_st->getSourceRange()),
        st_content);
}