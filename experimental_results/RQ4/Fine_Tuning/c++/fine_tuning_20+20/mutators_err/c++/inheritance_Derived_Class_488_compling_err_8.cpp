//header file
#pragma once
#include "Mutator_base.h"

/**
 * Construct_Multiple-inheritance_Derived_Class_488
 */ 
class MutatorFrontendAction_488 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(488)
    
    private:
      class MutatorASTConsumer_488 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_488(Rewriter &R) : TheRewriter(R) {}
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
          vector<string> base_classes;
          vector<string> base_access;
      };
    };

//source file
#include "../include/construct_multiple-inheritance_derived_class_488"

// ========================================================================================================
#define MUT488_OUTPUT 1

void MutatorFrontendAction_488::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    //Get the source code text of target node
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    auto bases = DL->bases();
    if (bases.size() < 2)
      return;
    base_classes.clear();
    base_access.clear();
    for (auto base : bases) {
      auto base_type = base.getType();
      auto base_decl = base_type->getAsCXXRecordDecl();
      base_classes.push_back(base_decl->getNameAsString());
      auto base_access_spec = base.getAccessSpecifier();
      if (base_access_spec == AccessSpecifier::AS_none) {
        if (DL->isStruct())
          base_access.push_back("public");
        else
          base_access.push_back("private");
      } else if (base_access_spec == AccessSpecifier::AS_public)
        base_access.push_back("public");
      else if (base_access_spec == AccessSpecifier::AS_protected)
        base_access.push_back("protected");
      else if (base_access_spec == AccessSpecifier::AS_private)
        base_access.push_back("private");
      else
        base_access.push_back("private");
    }
    //Perform mutation on the source code text by applying string replacement
    string alias_template = "template<size_t I> using Base=typename std::tuple_element<I,std::tuple<";
    for (size_t i = 0; i < base_classes.size(); ++i) {
      if (i)
        alias_template += ',';
      alias_template += base_classes[i];
    }
    alias_template += ">>::type;";
    llvm::outs() << alias_template << '\n';
    content.insert(content.find('{'), "/*mut488*/" + alias_template);
    //Replace the original AST node with the mutated one
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                 "Derived2")) { // If there is a return 0 statement in the function, replace the return value with the inserted static const
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    //Get the source code text of target node
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    auto bases = DL->bases();
    if (bases.size() < 2)
      return;
    //Perform mutation on the source code text by applying string replacement
    string base_init = "";
    for (size_t i = 0; i < base_classes.size(); ++i) {
      if (i)
        base_init += ',';
      base_init += base_access[i] + " Base<" + std::to_string(i) + '>';
    }
    llvm::outs() << base_init << '\n';
    content.replace(content.find(':'),
                    content.find('{') - content.find(':') - 1, base_init);
    //Replace the original AST node with the mutated one
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
  }
}

void MutatorFrontendAction_488::MutatorASTConsumer_488::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto derived_matcher =
        cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    auto derived_matcher2 =
        cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived2");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(derived_matcher2, &callback);
    matchFinder.matchAST(Context);
}