//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Using_Declaration_In_Class_97
 */ 
class MutatorFrontendAction_97 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(97)
    
    private:
      class MutatorASTConsumer_97 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_97(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<const clang::CXXRecordDecl *> cur_classes;
      };
    };

//source file
#include "../include/insert_using_declaration_in_class_97"

// ========================================================================================================
#define MUT97_OUTPUT 1

void MutatorFrontendAction_97::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    cur_classes.push_back(DL);
  } else if (auto *UL =
                 Result.Nodes.getNodeAs<clang::UsingDecl>("UsingDecl")) {
    if (!UL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   UL->getBeginLoc()))
      return;

    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), UL->getSourceRange());
    llvm::outs() << content << '\n';
    if (UL->getQualifier() == nullptr) {
      llvm::outs() << "Mut97: using declaration has no qualifer!\n";
      return;
    }
    auto qualifer =
        stringutils::rangetoStr(*(Result.SourceManager), UL->getQualifier());
    if (UL->getQualifier()->getKind() == clang::NestedNameSpecifier::Namespace) {
      content = qualifer + " :: " + content;
    } else if (UL->getQualifier()->getKind() ==
               clang::NestedNameSpecifier::NamespaceAlias) {
      auto alias = qualifer;
      llvm::outs() << alias << '\n';
      auto ns = Result.Context->getTranslationUnitDecl()->lookup(alias);
      if (ns.empty()) {
        llvm::outs() << "Mut97: cannot find the namespace alias!\n";
        return;
      }
      auto ns_decl = dyn_cast<NamespaceAliasDecl>(ns[0]);
      if (ns_decl == nullptr) {
        llvm::outs() << "Mut97: cannot find the namespace alias!\n";
        return;
      }
      auto ns_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                ns_decl->getQualifier());
      auto ns_name = ns_decl->getAliasedNamespace()->getNameAsString();
      content = ns_content + " :: " + ns_name + " :: " + content;
    } else if (UL->getQualifier()->getKind() ==
               clang::NestedNameSpecifier::TypeSpec) {
      int dice = getrandom::getRandomIndex(2);
      if (dice == 0) {
        content = content.substr(content.rfind(':') + 1);
      } else if (dice == 1) {
        content = qualifer + " :: ";
      } else if (dice == 2) {
        content = "";
      }
    }
    content = "/*mut97*/using " + content + ";\n";
    llvm::outs() << content << '\n';
    size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
    auto target = cur_classes[index];
    Rewrite.ReplaceText(target->getEndLoc(), 0, content);
  }
}

void MutatorFrontendAction_97::MutatorASTConsumer_97::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto usingdecl_matcher = usingDecl().bind("UsingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(usingdecl_matcher, &callback);
    matchFinder.matchAST(Context);
}