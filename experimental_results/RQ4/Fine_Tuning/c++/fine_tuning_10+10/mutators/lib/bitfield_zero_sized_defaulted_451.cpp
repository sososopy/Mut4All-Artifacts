//source file
#include "../include/Mutator_Bitfield_Zero_Sized_Defaulted_451.h"

// ========================================================================================================
#define MUT451_OUTPUT 1

void MutatorFrontendAction_451::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassWithBitfields")) {
        //Filter nodes in header files
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        
        if (!CL->isCompleteDefinition())
            return;

        currentClass = CL;

        //Get the source code text of target node
        auto classText = stringutils::rangetoStr(*(Result.SourceManager), CL->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        if (classText.find("unsigned : 0;") == std::string::npos) {
            classText.insert(classText.find_last_of('}'), "\n    unsigned : 0; // Zero-sized bit-field\n");
        }

        for (auto method : CL->methods()) {
            if (method->isDefaulted() && !method->isDeleted()) {
                auto methodText = stringutils::rangetoStr(*(Result.SourceManager), method->getSourceRange());
                if (methodText.find("= default;") == std::string::npos) {
                    methodText.append(" = default;");
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(method->getSourceRange()), methodText);
                }
            }
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), classText);
    }
}

void MutatorFrontendAction_451::MutatorASTConsumer_451::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(has(fieldDecl(isBitField()))).bind("ClassWithBitfields");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}