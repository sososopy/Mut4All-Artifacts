//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_NestedNameSpecifier_And_PointerToMember_413
 */ 
class MutatorFrontendAction_413 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(413)

private:
    class MutatorASTConsumer_413 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_413(Rewriter &R) : TheRewriter(R) {}
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
        const clang::NamespaceDecl *cur_ns;
        const clang::CXXRecordDecl *cur_class;
        const clang::CXXRecordDecl *cur_struct;
        const clang::CXXMethodDecl *cur_method;
        const clang::VarDecl *cur_var;
    };
};

//source file
#include "../include/Target_NestedNameSpecifier_And_PointerToMember_413.h"

// ========================================================================================================
#define MUT413_OUTPUT 1

void MutatorFrontendAction_413::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      cur_ns = NS;
    } else if (auto *CL =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if (CL->isStruct())
        cur_struct = CL;
      else
        cur_class = CL;
    } else if (auto *MD =
                   Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      cur_method = MD;
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Var")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      cur_var = VD;
    } else if (auto *NNS = Result.Nodes.getNodeAs<clang::NestedNameSpecifier>(
                   "NestedNameSpecifier")) {
      if (!NNS || !Result.Context->getSourceManager().isWrittenInMainFile(
                      NNS->getBeginLoc()))
        return;
      if (cur_ns == nullptr)
        return;
      if (cur_class == nullptr && cur_struct == nullptr)
        return;
      if (cur_method == nullptr)
        return;
      if (cur_var == nullptr)
        return;
      auto ns_name = cur_ns->getNameAsString();
      auto class_name = cur_class == nullptr
                            ? cur_struct->getNameAsString()
                            : cur_class->getNameAsString();
      auto method_name = cur_method->getNameAsString();
      auto var_name = cur_var->getNameAsString();
      auto nns_name = stringutils::rangetoStr(*(Result.SourceManager),
                                              NNS->getSourceRange());
      llvm::outs() << ns_name << " " << class_name << " " << method_name << " "
                   << var_name << " " << nns_name << "\n";
      if (nns_name != ns_name + "::" && nns_name != class_name + "::")
        return;
      string ins = "/*mut413*/";
      if (nns_name == ns_name + "::") {
        if (cur_class != nullptr) {
          ins += "void (" + class_name + "::*mut_413)() = &" + nns_name +
                 class_name + "::" + method_name + ";\n";
          ins += class_name + " mut_413_obj;\n";
          ins += "(mut_413_obj.*mut_413)();\n";
        } else {
          ins += "int " + class_name + "::*mut_413 = &" + nns_name +
                 class_name + "::" + var_name + ";\n";
          ins += class_name + " mut_413_obj;\n";
          ins += "mut_413_obj.*mut_413;\n";
        }
      } else if (nns_name == class_name + "::") {
        if (cur_class != nullptr) {
          ins += "void (" + class_name + "::*mut_413)() = &" + nns_name +
                 method_name + ";\n";
          ins += class_name + " mut_413_obj;\n";
          ins += "(mut_413_obj.*mut_413)();\n";
        } else {
          ins += "int " + class_name + "::*mut_413 = &" + nns_name + var_name +
                 ";\n";
          ins += class_name + " mut_413_obj;\n";
          ins += "mut_413_obj.*mut_413;\n";
        }
      }
      llvm::outs() << ins << "\n";
      Rewrite.ReplaceText(NNS->getBeginLoc(), 0, ins);
    }
  }
  
void MutatorFrontendAction_413::MutatorASTConsumer_413::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto ns_matcher = namespaceDecl().bind("Namespace");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto method_matcher = cxxMethodDecl().bind("Methods");
    auto var_matcher = varDecl().bind("Var");
    auto nns_matcher = nestedNameSpecifier().bind("NestedNameSpecifier");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(ns_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(method_matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.addMatcher(nns_matcher, &callback);
    matchFinder.matchAST(Context);
}