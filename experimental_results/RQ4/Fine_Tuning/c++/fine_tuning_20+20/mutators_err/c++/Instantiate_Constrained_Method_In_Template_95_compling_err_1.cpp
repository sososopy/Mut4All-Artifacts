//header file
#pragma once
#include "Mutator_base.h"

/**
 * Instantiate_Constrained_Method_In_Template_95
 */ 
class MutatorFrontendAction_95 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(95)

private:
    class MutatorASTConsumer_95 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_95(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::ClassTemplateDecl *> cur_templates;
        std::vector<std::string> cur_constraints;
        std::vector<std::string> cur_methods;
        std::vector<int> cur_indexes;
    };
};

//source file
#include "../include/instantiate_constrained_method_in_template_95.h"

// ========================================================================================================
#define MUT95_OUTPUT 1

void MutatorFrontendAction_95::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto DLname = DL->getNameAsString();
      if (DLname == "")
        return;
      llvm::outs() << DLname << '\n';
      auto spec = DL->getTemplatedDecl();
      auto methods = spec->methods();
      int index = -1;
      int cur = 0;
      for (auto method : methods) {
        if (method->hasBody() == false)
          continue;
        if (method->getNameAsString() == DLname)
          continue;
        if (method->isOverloadedOperator())
          continue;
        if (method->isVirtual() || method->isPure())
          continue;
        if (method->getAccess() != AccessSpecifier::AS_public)
          continue;
        if (method->doesThisDeclarationHaveABody() == false)
          continue;
        if (method->getDescribedFunctionTemplate() != nullptr)
          continue;
        if (method->getPrimaryTemplate() != nullptr)
          continue;
        if (method->getBeginLoc() == DL->getBeginLoc())
          continue;
        if (method->isConsteval())
          continue;
        if (method->isConstexpr())
          continue;
        if (method->isExplicitlyDefaulted())
          continue;
        if (method->isDeleted())
          continue;
        if (method->isStatic())
          continue;
        if (method->isOverloadedOperator())
          continue;
        if (method->getOverloadedOperator() != OverloadedOperatorKind::OO_None)
          continue;
        if (method->isUserProvided() == false)
          continue;
        if (method->isDefined() == false)
          continue;
        if (method->hasAttrs())
          continue;
        auto constraints = method->getTemplateSpecializationArgs();
        if (constraints == nullptr)
          continue;
        auto constraint = constraints->asArray();
        if (constraint.size() == 0)
          continue;
        llvm::outs() << method->getNameAsString() << '\n';
        for (auto con : constraint) {
          if (con.getKind() == TemplateArgument::Type) {
            auto con_type = con.getAsType();
            if (con_type->isRecordType()) {
              index = cur;
              break;
            }
          }
        }
        cur++;
      }
      if (index == -1)
        return;
      llvm::outs() << index << '\n';
      cur_templates.push_back(DL);
      cur_indexes.push_back(index);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "Templates2")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto DLname = DL->getNameAsString();
      if (DLname == "")
        return;
      llvm::outs() << DLname << '\n';
      auto spec = DL->getTemplatedDecl();
      auto methods = spec->methods();
      int index = -1;
      int cur = 0;
      for (auto method : methods) {
        if (method->hasBody() == false)
          continue;
        if (method->getNameAsString() == DLname)
          continue;
        if (method->isOverloadedOperator())
          continue;
        if (method->isVirtual() || method->isPure())
          continue;
        if (method->getAccess() != AccessSpecifier::AS_public)
          continue;
        if (method->doesThisDeclarationHaveABody() == false)
          continue;
        if (method->getDescribedFunctionTemplate() != nullptr)
          continue;
        if (method->getPrimaryTemplate() != nullptr)
          continue;
        if (method->getBeginLoc() == DL->getBeginLoc())
          continue;
        if (method->isConsteval())
          continue;
        if (method->isConstexpr())
          continue;
        if (method->isExplicitlyDefaulted())
          continue;
        if (method->isDeleted())
          continue;
        if (method->isStatic())
          continue;
        if (method->isOverloadedOperator())
          continue;
        if (method->getOverloadedOperator() != OverloadedOperatorKind::OO_None)
          continue;
        if (method->isUserProvided() == false)
          continue;
        if (method->isDefined() == false)
          continue;
        if (method->hasAttrs())
          continue;
        auto constraints = method->getTemplateSpecializationArgs();
        if (constraints == nullptr)
          continue;
        auto constraint = constraints->asArray();
        if (constraint.size() == 0)
          continue;
        llvm::outs() << method->getNameAsString() << '\n';
        for (auto con : constraint) {
          if (con.getKind() == TemplateArgument::Type) {
            auto con_type = con.getAsType();
            if (con_type->isRecordType()) {
              index = cur;
              break;
            }
          }
        }
        cur++;
      }
      if (index == -1)
        return;
      llvm::outs() << index << '\n';
      auto target = methods[index];
      auto constraints = target->getTemplateSpecializationArgs();
      auto constraint = constraints->asArray();
      string cons = "";
      for (auto con : constraint) {
        if (con.getKind() == TemplateArgument::Type) {
          auto con_type = con.getAsType();
          if (con_type->isRecordType()) {
            cons += "int";
          } else {
            cons += con_type.getAsString();
          }
        }
      }
      cur_constraints.push_back(cons);
      cur_methods.push_back(target->getNameAsString());
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Records")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto DLname = DL->getNameAsString();
      if (DLname == "")
        return;
      llvm::outs() << DLname << '\n';
      for (std::size_t j = 0; j < cur_templates.size(); ++j) {
        if (DLname == cur_templates[j]->getNameAsString()) {
          auto target = DL;
          auto methods = target->methods();
          auto index = cur_indexes[j];
          auto target_method = methods[index];
          auto constraints = target_method->getTemplateSpecializationArgs();
          auto constraint = constraints->asArray();
          string cons = "";
          for (auto con : constraint) {
            if (con.getKind() == TemplateArgument::Type) {
              auto con_type = con.getAsType();
              if (con_type->isRecordType()) {
                cons += "int";
              } else {
                cons += con_type.getAsString();
              }
            }
          }
          llvm::outs() << cons << '\n';
          cur_constraints.push_back(cons);
          cur_methods.push_back(target_method->getNameAsString());
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Records2")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto DLname = DL->getNameAsString();
      if (DLname == "")
        return;
      llvm::outs() << DLname << '\n';
      for (std::size_t j = 0; j < cur_templates.size(); ++j) {
        if (DLname == cur_templates[j]->getNameAsString()) {
          auto target = DL;
          auto methods = target->methods();
          auto index = cur_indexes[j];
          auto target_method = methods[index];
          auto constraints = target_method->getTemplateSpecializationArgs();
          auto constraint = constraints->asArray();
          string cons = "";
          for (auto con : constraint) {
            if (con.getKind() == TemplateArgument::Type) {
              auto con_type = con.getAsType();
              if (con_type->isRecordType()) {
                cons += "int";
              } else {
                cons += con_type.getAsString();
              }
            }
          }
          llvm::outs() << cons << '\n';
          cur_constraints.push_back(cons);
          cur_methods.push_back(target_method->getNameAsString());
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>(
                   "Trans")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (cur_constraints.empty())
        return;
      for (std::size_t j = 0; j < cur_constraints.size(); ++j) {
        string ins = "template void " + cur_templates[j]->getNameAsString() +
                     "<" + cur_constraints[j] + ">::" + cur_methods[j] + "();";
        llvm::outs() << ins << '\n';
        Rewrite.ReplaceText(DL->getEndLoc(), 0, "\n/*mut95*/" + ins + "\n");
      }
    }
}

void MutatorFrontendAction_95::MutatorASTConsumer_95::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto matcher2 = classTemplateDecl().bind("Templates2");
    auto matcher3 = cxxRecordDecl().bind("Records");
    auto matcher4 = cxxRecordDecl().bind("Records2");
    auto matcher5 = translationUnitDecl().bind("Trans");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.addMatcher(matcher3, &callback);
    matchFinder.addMatcher(matcher4, &callback);
    matchFinder.addMatcher(matcher5, &callback);
    matchFinder.matchAST(Context);
}