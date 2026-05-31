//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Exception_Specification_In_Class_Or_Struct_284
 */ 
class MutatorFrontendAction_284 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(284)

private:
    class MutatorASTConsumer_284 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_284(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_exception_specification_in_class_or_struct_284.h"

// ========================================================================================================
#define MUT284_OUTPUT 1

void MutatorFrontendAction_284::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
        return;
      if (!DL->isCompleteDefinition())
        return;
      cur_classes.push_back(DL);
    } else if (auto *MT =
                   Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->hasBody() == false)
        return;
      if (MT->hasDynamicExceptionSpec()) {
        auto cur = stringutils::rangetoStr(*(Result.SourceManager),
                                           MT->getSourceRange());
        if (cur.find("throw()") != string::npos) {
          cur = stringutils::strReplace(cur, "throw()", "noexcept");
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(MT->getSourceRange()), cur);
        } else {
          auto target = stringutils::rangetoStr(
              *(Result.SourceManager), MT->getExceptionSpecSourceRange());
          if (target.find("throw(") != string::npos) {
            target = stringutils::strReplace(target, "throw(", "");
            target = stringutils::strReplace(target, ")", "");
            std::vector<string> types;
            llvm::StringRef target_ref = target;
            target_ref.split(types, ',');
            std::vector<string> new_types;
            for (auto type : types) {
              type = stringutils::strTrim(type);
              if (type != "int")
                new_types.push_back(type);
            }
            if (new_types.empty())
              new_types.push_back("int");
            target = "throw(";
            for (std::size_t j = 0; j < new_types.size(); ++j) {
              if (j)
                target += ",";
              target += new_types[j];
            }
            target += ")";
            llvm::outs() << target << '\n';
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(MT->getExceptionSpecSourceRange()),
                target);
          }
        }
      } else if (MT->hasNoexceptExceptionSpec()) {
        auto cur = stringutils::rangetoStr(*(Result.SourceManager),
                                           MT->getSourceRange());
        if (cur.find("noexcept") != string::npos) {
          cur = stringutils::strReplace(cur, "noexcept", "throw(int)");
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(MT->getSourceRange()), cur);
        }
      }
    }
  }
  
void MutatorFrontendAction_284::MutatorASTConsumer_284::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto method_matcher = cxxMethodDecl().bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(method_matcher, &callback);
    matchFinder.matchAST(Context);
}