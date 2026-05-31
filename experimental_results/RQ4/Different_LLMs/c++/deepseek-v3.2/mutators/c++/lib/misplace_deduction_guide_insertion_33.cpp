//source file
#include "../include/Misplace_Deduction_Guide_Insertion_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ConstructorDecl")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;
        const CXXRecordDecl* parent = CT->getParent();
        if (!parent || !parent->isTemplated())
            return;
        if (CT->isCopyOrMoveConstructor() || CT->isDefaultConstructor())
            return;
        candidateConstructors.push_back(CT);
    }
    else if (auto *TC = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
        if (!TC || !Result.Context->getSourceManager().isWrittenInMainFile(TC->getLocation()))
            return;
        if (!TC->isTemplated() || !TC->isCompleteDefinition())
            return;
        candidateTemplateClasses.push_back(TC);
    }
    else if (auto *MC = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("MutateClass")) {
        if (!MC || !Result.Context->getSourceManager().isWrittenInMainFile(MC->getLocation()))
            return;
        if (!MC->isTemplated() || !MC->isCompleteDefinition())
            return;
        if (candidateConstructors.empty() || candidateTemplateClasses.empty())
            return;
        size_t ctorIdx = getrandom::getRandomIndex(candidateConstructors.size() - 1);
        size_t classIdx = getrandom::getRandomIndex(candidateTemplateClasses.size() - 1);
        const CXXConstructorDecl* selectedCtor = candidateConstructors[ctorIdx];
        const CXXRecordDecl* selectedClass = candidateTemplateClasses[classIdx];
        if (selectedCtor->getParent() == MC)
            return;
        if (selectedClass == MC)
            return;
        const TemplateDecl* ctorTemplate = selectedCtor->getParent()->getDescribedClassTemplate();
        if (!ctorTemplate)
            return;
        const TemplateParameterList* ctorParams = ctorTemplate->getTemplateParameters();
        std::string paramStr;
        if (ctorParams && ctorParams->size() > 0) {
            paramStr = "template<";
            for (unsigned i = 0; i < ctorParams->size(); ++i) {
                if (i > 0) paramStr += ", ";
                if (ctorParams->getParam(i)->isTemplateParameter())
                    paramStr += "typename ";
                paramStr += ctorParams->getParam(i)->getNameAsString();
            }
            paramStr += ">\n";
        }
        std::string ctorName = selectedCtor->getParent()->getNameAsString();
        std::string newDecl = paramStr + ctorName + "(";
        unsigned numParams = selectedCtor->getNumParams();
        for (unsigned i = 0; i < numParams; ++i) {
            if (i > 0) newDecl += ", ";
            QualType paramType = selectedCtor->getParamDecl(i)->getType();
            std::string typeStr = paramType.getAsString();
            newDecl += typeStr + " " + selectedCtor->getParamDecl(i)->getNameAsString();
        }
        newDecl += ") : ";
        for (unsigned i = 0; i < numParams; ++i) {
            if (i > 0) newDecl += ", ";
            newDecl += selectedCtor->getParamDecl(i)->getNameAsString() + "(" + selectedCtor->getParamDecl(i)->getNameAsString() + ")";
        }
        newDecl += " {}\n";
        newDecl = "/*mut33*/" + newDecl;
        Rewrite.InsertTextBefore(MC->getEndLoc(), newDecl);
    }
}
  
void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher constructorMatcher = cxxConstructorDecl(isUserProvided()).bind("ConstructorDecl");
    DeclarationMatcher templateClassMatcher = cxxRecordDecl(isTemplateInstantiation(), isDefinition()).bind("TemplateClass");
    DeclarationMatcher mutateClassMatcher = cxxRecordDecl(isTemplateInstantiation(), isDefinition()).bind("MutateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(constructorMatcher, &callback);
    matchFinder.addMatcher(templateClassMatcher, &callback);
    matchFinder.addMatcher(mutateClassMatcher, &callback);
    matchFinder.matchAST(Context);
}