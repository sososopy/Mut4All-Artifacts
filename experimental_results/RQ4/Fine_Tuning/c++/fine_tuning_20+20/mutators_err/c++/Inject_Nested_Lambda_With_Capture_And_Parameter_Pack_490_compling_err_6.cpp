//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Nested_Lambda_With_Capture_And_Parameter_Pack_490
 */ 
class MutatorFrontendAction_490 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(490)

private:
    class MutatorASTConsumer_490 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_490(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::FunctionDecl *> cur_functions;
    };
};

//source file
#include "../include/Inject_Nested_Lambda_With_Capture_And_Parameter_Pack_490.h"

// ========================================================================================================
#define MUT490_OUTPUT 1

void MutatorFrontendAction_490::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FL->getLocation()))
        return;
      if (FL->isTemplateInstantiation())
        return;
      if (!FL->hasBody())
        return;
      cur_functions.push_back(FL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isTemplateInstantiation())
        return;
      if (!DL->hasBody())
        return;
      cur_functions.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructors")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isTemplateInstantiation())
        return;
      if (!DL->hasBody())
        return;
      cur_functions.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Destructors")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isTemplateInstantiation())
        return;
      if (!DL->hasBody())
        return;
      cur_functions.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXConversionDecl>("Conversions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isTemplateInstantiation())
        return;
      if (!DL->hasBody())
        return;
      cur_functions.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("LambdaMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isTemplateInstantiation())
        return;
      if (!DL->hasBody())
        return;
      if (DL->getParent()->isLambda()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getSourceRange());
        if (content.find("...") != string::npos) {
          llvm::outs() << "lambda method with pack\n";
          return;
        }
      }
      cur_functions.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("LambdaFunctions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isTemplateInstantiation())
        return;
      if (!DL->hasBody())
        return;
      if (DL->getParent()->isLambda()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getSourceRange());
        if (content.find("...") != string::npos) {
          llvm::outs() << "lambda function with pack\n";
          return;
        }
      }
      cur_functions.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("LambdaConstructors")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isTemplateInstantiation())
        return;
      if (!DL->hasBody())
        return;
      if (DL->getParent()->isLambda()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getSourceRange());
        if (content.find("...") != string::npos) {
          llvm::outs() << "lambda constructor with pack\n";
          return;
        }
      }
      cur_functions.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("LambdaDestructors")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isTemplateInstantiation())
        return;
      if (!DL->hasBody())
        return;
      if (DL->getParent()->isLambda()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getSourceRange());
        if (content.find("...") != string::npos) {
          llvm::outs() << "lambda destructor with pack\n";
          return;
        }
      }
      cur_functions.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXConversionDecl>("LambdaConversions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isTemplateInstantiation())
        return;
      if (!DL->hasBody())
        return;
      if (DL->getParent()->isLambda()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getSourceRange());
        if (content.find("...") != string::npos) {
          llvm::outs() << "lambda conversion with pack\n";
          return;
        }
      }
      cur_functions.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("LambdaClasses")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isLambda()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getSourceRange());
        if (content.find("...") != string::npos) {
          llvm::outs() << "lambda class with pack\n";
          return;
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isLambda()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getSourceRange());
        if (content.find("...") != string::npos) {
          llvm::outs() << "lambda class with pack\n";
          return;
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("LambdaDecls")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isLambda()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getSourceRange());
        if (content.find("...") != string::npos) {
          llvm::outs() << "lambda class with pack\n";
          return;
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("LambdaDefs")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isLambda()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL->getSourceRange());
        if (content.find("...") != string::npos) {
          llvm::outs() << "lambda class with pack\n";
          return;
        }
      }
    }
    if (cur_functions.empty())
      return;
    auto DL = cur_functions[llvm::sys::Process::GetRandomNumber() %
                            cur_functions.size()];
    auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                           DL->getSourceRange());
    if (DL->getBody()) {
      auto body = DL->getBody();
      auto body_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  body->getSourceRange());
      if (body_content.find("[]") != string::npos) {
        llvm::outs() << "already has lambda\n";
        return;
      }
    }
    if (content.find("...") != string::npos) {
      llvm::outs() << "function with pack\n";
      return;
    }
    if (DL->isLambda()) {
      llvm::outs() << "lambda function\n";
      return;
    }
    if (DL->isTemplateInstantiation()) {
      llvm::outs() << "template instantiation\n";
      return;
    }
    if (DL->isTemplated()) {
      llvm::outs() << "templated\n";
      return;
    }
    if (DL->isDependentContext()) {
      llvm::outs() << "dependent context\n";
      return;
    }
    if (DL->isDependentType()) {
      llvm::outs() << "dependent type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependentScopeDecl()) {
      llvm::outs() << "dependent scope decl\n";
      return;
    }
    if (DL->isDependentScopeType()) {
      llvm::outs() << "dependent scope type\n";
      return;
    }
    if (DL->isDependent