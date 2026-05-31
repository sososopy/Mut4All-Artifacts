//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Inherited_Constructor_414
 */ 
class MutatorFrontendAction_414 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(414)
    
    private:
      class MutatorASTConsumer_414 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_414(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<const clang::CXXRecordDecl *> base_classes;
      };
    };

//source file
#include "../include/specialize_inherited_constructor_414"

// ========================================================================================================
#define MUT414_OUTPUT 1

void MutatorFrontendAction_414::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
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
    for (std::size_t j = 0; j < base_classes.size(); ++j) {
      if (DL->isDerivedFrom(base_classes[j])) {
        auto ctors = base_classes[j]->ctors();
        for (auto ctor : ctors) {
          if (ctor->getAccess() == AccessSpecifier::AS_private)
            continue;
          auto params = ctor->parameters();
          std::string param_type = "";
          for (auto param : params) {
            if (param_type != "")
              param_type += ",";
            param_type += param->getType().getAsString();
          }
          auto base_name = base_classes[j]->getNameAsString();
          auto derived_name = DL->getNameAsString();
          if (param_type == "")
            param_type = "void";
          auto ins = "/*mut414*/using " + base_name + " :: " + base_name +
                     ";\ntemplate<>\n" + derived_name + "(" + param_type +
                     ");\n";
          llvm::outs() << ins;
          Rewrite.ReplaceText(DL->getEndLoc(), 0, ins);
          return;
        }
      }
    }
  } else if (auto *BL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                 "Base")) { // If there is a return 0 statement in the function, replace the return value with the inserted static const
    if (!BL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   BL->getLocation()))
      return;
    if ((!BL->isStruct() && !BL->isClass()) || BL->isLambda())
      return;
    base_classes.push_back(BL);
  }
}

void MutatorFrontendAction_414::MutatorASTConsumer_414::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto base_matcher = cxxRecordDecl().bind("Base");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.matchAST(Context);
}